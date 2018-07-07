#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
/* 配列の最大値を求める関数
    n: 要素数
    x: 対象配列
    戻り値: 最大値
*/
float amax(int n, const float *x) {
    float res;
    for (int j = 0; j < n; j++) {
        if (j == 0 || x[j] > res) res = x[j];
    }
    return res;
}
/* 配列のコピー
    n: 配列のサイズ
    src: コピー元
    dist: コピー先
*/
void acopy(const int n, const float *src, float *dist) {
    memcpy(dist, src, sizeof(float)*n);
}
/* 値の入れ替え関数
    j, k: 入れ替え対象の変数
*/
void swap(int *j, int *k) {
    int t = *k;
    *k = *j;
    *j = t;
}
/* 乱数取得
    irand: [base:max)の整数乱数を返す
    frand: [base:max]の小数乱数を返す
*/
int irand(const int max, const int base) {
    return (int)(rand() / (1.0 + RAND_MAX) * (max - base) + base);
}
float frand(const float max, const float base) {
    return rand() * 1.0 / RAND_MAX * (max - base) + base;
}
/* BoxMuller法を用いたガウス分布に基づいた乱数生成
    avg: 平均
    sd: 標準偏差
    戻り値: ガウス分布に基づく乱数
*/
float frand_BoxMuller(const float avg, const float sd) {
    float u1 = rand() / (1.0 + RAND_MAX);
    float u2 = rand() / (1.0 + RAND_MAX);
    return avg + sd * pow(-2.0 * log(u1), 0.5) * cos(2.0 * M_PI * u2);
}
/* 配列の除算
    n: 配列のサイズ
    x: 商
    o: 操作配列
*/
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
/* ガウス分布を用いた配列初期化関数
    n: 配列のサイズ
    o: 操作配列
    input_size: 入力次元数
*/
void rand_init_bm(int n, float * o, const float input_size) {
    for (int j = 0; j < n; j++) {
        o[j] = frand_BoxMuller(0, pow(2.0 / input_size, 0.5));
    }
}

void shuffle(int n, int *x) {
    for (int j = 0; j < n; j++) {
        int k = irand(n, 0);
        swap(&x[j], &x[k]);
    }
}

float cross_entropy_error(const float * y, int t) {
    return - log(y[t] + 1e-7);
}

// ファイル読込・書込
void save(const char *filename, int m, int n, const float *A, const float *b) {
    FILE *fpp;

    fpp = fopen( filename, "wb" );
    if( fpp == NULL )
    {
        printf("Error: Can't save\n");
        return;
    }

    fwrite(A, sizeof(float), m * n, fpp);
    fwrite(b, sizeof(float), m, fpp);

    fclose(fpp);
}
void load(const char *filename, int m, int n, float *A, float *b) {
    FILE *fpp;

    fpp = fopen( filename, "rb" );
    if( fpp == NULL )
    {
        printf("Error: Can't save\n");
        return;
    }

    fread(A, sizeof(float), m * n, fpp);
    fread(b, sizeof(float), m, fpp);

    fclose(fpp);
}