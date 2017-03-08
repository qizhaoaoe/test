#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <pthread.h>

#define NUM_THREAD 2 // 线程数
#define WIDTH 10

using namespace std;

int task(int block_size, int index, double InitMatrix[][WIDTH]){

    bool stable_flag = false;
    double ResultMatrix[][WIDTH];
    memcpy(ResultMatrix, InitMatrix, sizeof(InitMatrix))
    while (!stable_flag) {
        stable_flag = true;
        for (int i = 1; i <= block_size; ++ i) {
            for (int j = start; j <= ends; ++ j) {
                if(j != block_size)
                ResultMatrix[i][j] = 0.25 * (InitMatrix[i - 1][j] + InitMatrix[i + 1][j] + \
                        InitMatrix[i][j - 1] + InitMatrix[i][j + 1]);
                else{////有问题
                    ResultMatrix[i][j] = 0.25 * (InitMatrix[i - 1][j] + InitMatrix[i + 1][j] + \
                        InitMatrix[i][j - 1] + InitMatrix[i][j + 1]);
                }
                //如果是块的边缘，将其复制到buffer
                if(j == block_size)
                    memcpy(buffer[][index], Result[][j], sizeof(Result[][j]));
                if(j == ends - 1)
                if (fabs(ResultMatrix[i][j] - InitMatrix[i][j]) > pow(10, -3)) {
                    stable_flag = false;
                }
            }
        }
        for (int i = start; i <= ends; ++ i) {
            for (int j = start; j <= ends; ++ j) {
                InitMatrix[i][j] = ResultMatrix[i][j];
            }
        }
    }
    for (int i = 1; i < (WIDTH - 1); ++ i) {
        for (int j = 1; j < (WIDTH - 1); ++ j) {
            InitMatrix[i][j] = ResultMatrix[i][j];
            cout << InitMatrix[i][j] << " ";
        }
        cout << endl;
    }

    int counter = 0;
    for (int i = 1; i < (WIDTH - 1); ++ i) {
        for (int j = 1; j < (WIDTH - 1); ++ j) {
            if ((InitMatrix[i][j] - 5.5) > pow(10, -3)) {
                counter ++;
            }
        }
    }

    return counter;
}

int main (int argc, char **argv)
{
    double InitMatrix[WIDTH][WIDTH], ResultMatrix[WIDTH][WIDTH];
    double borders[WIDTH][(NUM_THREAD-1)*2];
    int total = 0;
    for (int i = 0; i < WIDTH; ++ i) {
        for (int j = 0; j < WIDTH; ++ j) {
            if (i == 0 || j == 0 || i == (WIDTH - 1) || j == (WIDTH - 1)) {
                InitMatrix[i][j] = ResultMatrix[i][j] = static_cast<double>(max(i,j));
            }
            else InitMatrix[i][j] = ResultMatrix[i][j] = 0;
        }
    }
    //将任务分块
    int block_size = (WIDTH-2) / NUM_THREAD;
    pthread_t tids[NUM_THREADS]; // thread id
    double buffer[WIDTH][NUM_THREAD] = {0};
    //create thread
    for(int i = 0; i < NUM_THREADS; i++){
        int start, ends;
        double blockMatrix[WIDTH][NUM_THREADS];
        start = i * block_size;
        ends = start + block_size;

        for(int k = 0; k < WIDTH; k++){
            for(int j = start; j <= ends+1; j++){
                blockMatrix[k][j] = InitMatrix[k][j];
            }
        }
        int ret = pthread_create(&tids[i], NULL, i, blockMatrix);
        total += ret;
    }





    cout << "\tResult :" << total << endl;

    return 0;
}
