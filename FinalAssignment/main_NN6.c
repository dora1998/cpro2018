#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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

// 独自補助関数
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
// 配列のコピー
void acopy(const int n, const float *src, float *dist) {
    memcpy(dist, src, sizeof(float)*n);
}
// 値の入れ替え
void swap(int *j, int *k) {
    int t = *k;
    *k = *j;
    *j = t;
}
// 乱数取得
int irand(const int max, const int base) {
    return (int)(rand() / (1.0 + RAND_MAX) * (max - base) + base);
}
float frand(const float max, const float base) {
    return rand() / (1.0 + RAND_MAX) * (max - base) + base;
}
// 配列の除算
void divide(int n, float x, float * o) {
    for (int j = 0; j < n; j++) {
        o[j] /= x;
    }
}

// 補助関数
void add(int n, const float * x, float * o) {
    for (int j = 0; j < n; j++) {
        o[j] += x[j];
    }
}
void scale(int n, float x, float * o) {
    for (int j = 0; j < n; j++) {
        o[j] *= x;
    }
}
void init(int n, float x, float * o) {
    for (int j = 0; j < n; j++) {
        o[j] = x;
    }
}
void rand_init(int n, float * o) {
    for (int j = 0; j < n; j++) {
        o[j] = frand(1, -1);
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

// 教材よりyを引数に追加して取得できるように
int inference3(const float * A, const float * b, const float * x, float * y) {
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


void shuffle(int n, int *x) {
    for (int j = 0; j < n; j++) {
        int k = irand(n, 0);
        swap(&x[j], &x[k]);
    }
}

float loss(const float * y, int t) {
    return - log(y[t] + 1e-7);
}


// テスト
int main() {
    srand(time(NULL));

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

    // 1~3
    int epoc = 10;
    int minipatch_n = 100;
    int study = train_count / minipatch_n;
    float study_rate = 0.3;

    // 4.
    float * A = malloc(sizeof(float)*784*10);
    float * b = malloc(sizeof(float)*10);
    rand_init(M * N, A);
    rand_init(M, b);

    for (int j = 0; j < epoc; j++) {
        // (a)
        int *index = malloc(sizeof(int)*train_count);
        for(int k = 0; k < train_count; k++) {
            index[k] = k;
        }
        shuffle(train_count, index);

        // (b)
        float * a_dA = malloc(sizeof(float)*784*10);
        float * a_db = malloc(sizeof(float)*10);
        for (int k = 0; k < study; k++) {
            init(784 * 10, 0, a_dA);
            init(10, 0, a_db);

            for (int l = 0; l < minipatch_n; l++) {
                float * y = malloc(sizeof(float)*10);
                float * dA = malloc(sizeof(float)*784*10);
                float * db = malloc(sizeof(float)*10);
                backward3(A, b, train_x + 784*index[k * minipatch_n + l], train_y[index[k * minipatch_n + l]], y, dA, db);
                add(M * N, dA, a_dA);
                add(M, db, a_db);               
            }
            divide(M * N, minipatch_n, a_dA);
            divide(M, minipatch_n, a_db);
            scale(M * N, - study_rate, a_dA);
            scale(M, - study_rate, a_db);
            add(M * N, a_dA, A);
            add(M, a_db, b);
        }

        // (c) cf)補題7
        int sum = 0;
        float avg_loss = 0;
        for(int k = 0; k < test_count; k++) {
            float y[10];
            if(inference3(A, b, test_x + k*width*height, y) == test_y[k]) {
                sum++;
            }
            avg_loss += loss(y, test_y[k]);
        }
        avg_loss /= 1.0 * test_count;
        printf("[Epoc %d]\n", j + 1);
        printf("Correct: %f%%, Loss Avg: %f\n", sum * 100.0 / test_count, avg_loss);
    }
    return 0;
}