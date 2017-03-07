#include <iostream>
#include <algorithm>
#include <cmath>

#define WIDTH 10

using namespace std;

int main (int argc, char **argv)
{
    double InitMatrix[WIDTH][WIDTH], ResultMatrix[WIDTH][WIDTH];

    for (int i = 0; i < WIDTH; ++ i) {
        for (int j = 0; j < WIDTH; ++ j) {
            if (i == 0 || j == 0 || i == (WIDTH - 1) || j == (WIDTH - 1)) {
                InitMatrix[i][j] = ResultMatrix[i][j] = static_cast<double>(max(i,j));
            }
        }
    }

    bool stable_flag = false;
    while (!stable_flag) {
        stable_flag = true;
        for (int i = 1; i < (WIDTH - 1); ++ i) {
            for (int j = 1; j < (WIDTH - 1); ++ j) {
                ResultMatrix[i][j] = 0.25 * (InitMatrix[i - 1][j] + InitMatrix[i + 1][j] + \
                        InitMatrix[i][j - 1] + InitMatrix[i][j + 1]);
                if (fabs(ResultMatrix[i][j] - InitMatrix[i][j]) > pow(10, -3)) {
                    stable_flag = false;
                }
            }
        }
        for (int i = 1; i < (WIDTH - 1); ++ i) {
            for (int j = 1; j < (WIDTH - 1); ++ j) {
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

    cout << "\tResult :" << counter << endl;

    return 0;
}
