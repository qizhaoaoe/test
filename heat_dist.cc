#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <pthread.h>
#define NUM_THREAD 2 // 线程数
#define WIDTH 10
#define block_size (WIDTH-2) / NUM_THREAD
using namespace std;

bool flag[10000];
double InitMatrix[WIDTH][WIDTH], ResultMatrix[WIDTH][WIDTH];
pthread_t tids[NUM_THREAD]; // thread id
bool all_stable = 0;

typedef struct
{
    int index;
	int start;
	int ends;
}Args;

void *task(void *_args){

   Args *args = (Args *)_args;

   while (!all_stable) {
        flag[args->index] = true;
        for (int i = 1; i < (WIDTH-1); i++) {
            for (int j = args->start; j < args->ends; j++) {
				ResultMatrix[i][j] = 0.25 * (InitMatrix[i][j - 1] + InitMatrix[i][j + 1] + InitMatrix[i - 1][j] + InitMatrix[i + 1][j]);
				if (fabs(ResultMatrix[i][j] - InitMatrix[i][j]) > pow(10, -3)) {
                    flag[args->index] = false;
                }
            }
        }
        //将结果复制到InitMatrix中

        for (int i = 1; i < (WIDTH-1); i++) {
            for (int j = args->start; j < args->ends; j++) {
                InitMatrix[i][j] = ResultMatrix[i][j];
			}
        }
        all_stable = true;
        for(int k = 0; k < NUM_THREAD; k++){
            all_stable &= flag[k];
        }
  }
    pthread_exit(0);
 }


int main()
{
	Args *args;
	int counter = 0;
    for (int i = 0;i < NUM_THREAD;i++){
            flag[i] = 0;
    }
	for (int i = 0; i < WIDTH; ++ i) {
        for (int j = 0; j < WIDTH; ++ j) {
            if (i == 0 || j == 0 || i == (WIDTH - 1) || j == (WIDTH - 1)) {
                InitMatrix[i][j] = ResultMatrix[i][j] = static_cast<double>(max(i,j));
            }
            else InitMatrix[i][j] = ResultMatrix[i][j] = 0;
        }
    }
    cout<<"InitMatrix:"<<endl;
	for (int i = 0; i < WIDTH; ++ i) {
        for (int j = 0; j < WIDTH; ++ j) {
          cout<<InitMatrix[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
    //create thread
    for(int i = 0; i < NUM_THREAD; i++){
        args  = (Args *)malloc(sizeof(Args));
        args->index = i;
        args->start = i * block_size + 1;
        args->ends = (i + 1) * block_size + 1;
        pthread_create(&tids[i], NULL, task, (void *)args);
    }

	for(int i = 0; i < NUM_THREAD; i++){
        pthread_join(tids[i], NULL);
	}

	cout<<"Result Matrix:"<<endl;
	for (int i = 1; i < WIDTH-1; ++ i) {
        for (int j = 1; j < WIDTH-1; ++ j) {
          cout<<ResultMatrix[i][j]<<" ";
          if ((ResultMatrix[i][j] - 5.5) > pow(10, -3)) {
            counter ++;
           }
         }
        cout<<endl;

    }
    cout<<"Counter = "<<counter<<endl;
    free(args);
    return 0;
}
