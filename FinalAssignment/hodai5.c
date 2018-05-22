#include <stdio.h>
#include "nn.h"

void print(int m, int n, const float *x) {
    for (int j = 0; j < m; j++) {
        for (int k = 0; k < n; k++) {
            printf("%.4f", x[j * n + k]);
            if (k != (n - 1)) printf(" ");
        }
        printf("\n");
    }
}
void fc(int m, int n, const float * x, const float * A, const float * b, float * y) {
    for (int j = 0; j < m; j++) {
        y[j] = b[j];
        for (int k = 0; k < n; k++) {
            y[j] += A[j * n + k] * x[k];
        }
    }
}
void relu(int n, const float * x, float * y) {
    for (int j = 0; j < n; j++) {
        y[j] = x[j] > 0 ? x[j] : 0;
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

int inference3(const float * A, const float * b, const float * x) {
    float y[10];
    const int M = 10, N = 784;
    fc(M, N, x, A, b, y);
    relu(M, y, y);
    softmax(M, y, y);

    int ym = 0;
    for (int j = 0; j < M; j++) {
        if (y[j] > y[ym] || j == 0) ym = j;
    }
    return ym;
}

// テスト
int main() {
    float *train_x = NULL;
    unsigned char *train_y = NULL;
    int train_count = -1;
    float *test_x = NULL;
    unsigned char *test_y = NULL;
    int test_count = -1;
    int width = -1;
    int height = -1;
    load_mnist(&train_x, &train_y, &train_count,
                &test_x, &test_y, &test_count,
                &width, &height);

    // これ以降，3層 NN の係数 A_784x10 および b_784x10 と，
    // 訓練データ train_x + 784*i (i=0,...,train_count-1), train_y[0]~train_y[train_count-1], // テストデータ test_x + 784*i (i=0,...,test_count-1), test_y[0]~test_y[test_count-1],
    // を使用することができる.

    int ans = inference3(A_784x10, b_784x10, train_x);
    printf("%d %d\n", ans, train_y[0]);
    return 0;
}