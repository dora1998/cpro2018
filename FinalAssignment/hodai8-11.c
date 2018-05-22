#include <stdio.h>
#include "nn.h"

#define M 10
#define N 784

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
    fc(M, N, x, A, b, y);
    relu(M, y, y);
    softmax(M, y, y);

    int ym = 0;
    for (int j = 0; j < M; j++) {
        if (y[j] > y[ym] || j == 0) ym = j;
    }
    return ym;
}

/*
誤差逆伝搬(Softmax 層)
    n: 10(m)
    *y, *dx: n個の配列
    t: 0-9の整数
*/
void softmaxwithloss_bwd(int n, const float * y, unsigned char t, float * dx) {
    for (int j = 0; j < n; j++) {
        int t_j = t == j ? 1 : 0;
        dx[j] = y[j] - t_j;
    }
}
/*
誤差逆伝搬(ReLU 層)
*/
void relu_bwd(int n, const float * x, const float * dy, float * dx) {
    for (int j = 0; j < n; j++) {
        dx[j] = x[j] > 0 ? dy[j] : 0;
    }
}
/* 
誤差逆伝搬(FC 層)
    m, n: 行列サイズ
    *x, *dx: n列の行列？
    *dy: m行の行列
    *A, *dA: m x n 行列
    *db: m行の行列
*/
void fc_bwd(int m, int n, const float * x, const float * dy, const float * A,
        float * dA, float * db, float * dx) {
    for (int j = 0; j < m; j++) {
        for (int k = 0; k < n; k++) {
            dA[j * n + k] = dy[j] * x[k];
        }
        db[j] = dy[j];
    }

    /*
    for (int j = 0; j < n; j++) {
        dx[j] = 0;
        for (int k = 0; k < m; k++) {
            dx[j] += A[k * n + j] * dy[k];
        }
    }*/
}

void acopy(int n, const float *src, float *dist) {
    memcpy(dist, src, sizeof(float)*n);
}
void backward3(const float * A, const float * b, const float * x, unsigned char t,
        float * y, float * dA, float * db) {
    float x_relu[M];
    fc(M, N, x, A, b, y);
    acopy(M, y, x_relu);
    relu(M, y, y);
    softmax(M, y, y);

    softmaxwithloss_bwd(M, y, t, y);
    relu_bwd(M, x_relu, y, y);
    fc_bwd(M, N, x, y, A, dA, db, y);
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

    float * y = malloc(sizeof(float)*10);
    float * dA = malloc(sizeof(float)*784*10);
    float * db = malloc(sizeof(float)*10);
    backward3(A_784x10, b_784x10, train_x + 784*8, train_y[8], y, dA, db);
    print(10, 784, dA);
    print(1, 10, db);
    return 0;
}