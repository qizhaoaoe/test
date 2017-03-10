#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <mutex>
#include <pthread.h>

#define NUM_THREAD 2 // 线程数
#define WIDTH 10

using namespace std;

mutex mytmx;
double buffer[WIDTH][NUM_THREAD];
int block_size = (WIDTH-2) / NUM_THREAD;
bool is_bufferNull = false;

typedef struct 
{
	//int block_size;
	int index;
	bool is_bufferNull;
	double **InitMatrix;
}Args;

void task(int index, double **InitMatrix){

    bool stable_flag = false;
	int counter = 0;
    double ResultMatrix[WIDTH][block_size+2];
    memcpy(ResultMatrix, InitMatrix, sizeof(InitMatrix));
    while (!stable_flag) {
        stable_flag = true;
		for (int j = 1; j <= block_size; ++ j) {
			//判断是否是边缘

			//左边缘且非第一个线程
			if(j == 1 && index != 0){
				if(mytmx.try_lock()){
					if(!is_bufferNull){
						for(int k = 1; k <= WIDTH-2; k++){
							InitMatrix[k][j] = buffer[k][index-1];
						}
					
					}
				mytmx.unlock();
				}
				//右边缘且非最后一个线程
				if(j == block_size && index != NUM_THREAD-1)
				{
					if(mytmx.try_lock()){
						if(!is_bufferNull){
							for(int k = 1; k <= WIDTH-2; k++){
								InitMatrix[k][j] = buffer[k][index+1];
							}
						}
						mytmx.unlock();
					}
				}
				
				
			}
			for (int i = 1; i <= block_size; ++ i) {
				ResultMatrix[i][j] = 0.25 * (InitMatrix[i - 1][j] + InitMatrix[i + 1][j] + InitMatrix[i][j - 1] + InitMatrix[i][j + 1]);
            }    
		}
         
		for (int j = 1; j <= block_size; ++ j) {
			for (int i = 1; i <= WIDTH-2; ++ i) {
				InitMatrix[i][j] = ResultMatrix[i][j];
			}
			//如果是块的边缘，将其复制到buffer
			if(mytmx.try_lock()){
				is_bufferNull = false;
				if(j == 1 && index != 0){
					for(int i =1; i <= WIDTH-2; i++){
						buffer[i][index-1] = InitMatrix[i][j];
					}
				}
				if(j == block_size && index != NUM_THREAD-1){
					for(int i = 1; i <= WIDTH-2; i++){
						buffer[i][index+1] = InitMatrix[i][j];
					}
				}
				mytmx.unlock();
			}
			
        }
		

    }
    /*for (int i = 1; i < (WIDTH - 1); ++ i) {
        for (int j = 1; j < (WIDTH - 1); ++ j) {
            InitMatrix[i][j] = ResultMatrix[i][j];
            cout << InitMatrix[i][j] << " ";
        }
        cout << endl;
    }*/
	
    for (int i = 1; i < (WIDTH - 1); ++ i) {

        for (int j = 1; j < (WIDTH - 1); ++ j) {
            if ((InitMatrix[i][j] - 5.5) > pow(10, -3)) {
                counter ++;
            }
        }
    }
    cout << "Thread" << index <<"\tResult :" << counter << endl;
    
}

int main (int argc, char **argv)
{
    double InitMatrix[WIDTH][WIDTH], ResultMatrix[WIDTH][WIDTH];
       
    pthread_t tids[NUM_THREAD]; // thread id
	bool is_bufferNull = true;
	Args *args = (Args *)malloc(sizeof(Args));

	for (int i = 0; i < WIDTH; ++ i) {
        for (int j = 0; j < WIDTH; ++ j) {
            if (i == 0 || j == 0 || i == (WIDTH - 1) || j == (WIDTH - 1)) {
                InitMatrix[i][j] = ResultMatrix[i][j] = static_cast<double>(max(i,j));
            }
            else InitMatrix[i][j] = ResultMatrix[i][j] = 0;
        }
    }

    //create thread
    for(int i = 0; i < NUM_THREAD; i++){
        int start, ends;
        double blockMatrix[WIDTH][block_size+2];
        start = i * block_size;
        ends = start + block_size;

        for(int k = 0; k < WIDTH; k++){
            for(int j = start; j <= ends+1; j++){
                blockMatrix[k][j] = InitMatrix[k][j];
            }
        }
		
		args->index = i;
		args->InitMartix = InitMatrix;
        int ret = pthread_create(&tids[i], NULL, task, args);
        if (ret != 0)
        {
			cout << "pthread_create error: error_code=" << ret << endl;  
        }
    }
	pthread_exit(NULL);//等各个线程退出后，进程才结束，否则进程强制结束了，线程可能还没反应过来；  
    return 0;
}
