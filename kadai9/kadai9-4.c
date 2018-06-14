#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void print(int m, int n, const float *x) {
    for (int j = 0; j < m; j++) {
        for (int k = 0; k < n; k++) {
            printf("%.4f", x[j * n + k]);
            if (k != (n - 1)) printf(" ");
        }
        printf("\n");
    }
}

// kadai6-6.c
void rand_init(int n, float *o) {
    for (int j = 0; j < n; j++) {
        o[j] = rand() * 1.0 / RAND_MAX * 2.0 - 1.0;
    }
}

/*
配列の最大値を求める関数
    int n: 要素数
    float *x: 対象配列
    return(float): 最大値
*/
float amax(int n, const float *x) {
    float res;
    for (int j = 0; j < n; j++) {
        if (j == 0 || x[j] > res) res = x[j];
    }
    return res;
}
void softmax(int n, const float * x, float * y) {
    float xm = amax(n, x);
    float xsum = 0;
    for (int j = 0; j < n; j++) {
        y[j] = exp(x[j] - xm);
        xsum += y[j];
    }
    for (int j = 0; j < n; j++) {
        y[j] /= xsum;
    }
}

// テスト
int main(int argc, char *argv[]) {
    srand(time(NULL));
    int n = 0;
    if (argc > 1) n = atoi(argv[1]);
    float *y = malloc(sizeof(float) * n);
    rand_init(n, y);
    print(1, n, y);
    softmax(n, y, y);
    print(1, n, y);
    return 0;
}