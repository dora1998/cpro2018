#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "nn.h"

const int debugMode = 0;

float *train_x = NULL;
unsigned char *train_y = NULL;
int train_count = -1;
float *test_x = NULL;
unsigned char *test_y = NULL;
int test_count = -1;
int width = -1;
int height = -1;

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
    return rand() * 1.0 / RAND_MAX * (max - base) + base;
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
    if (debugMode == 1) printf("  FC      input(%d x %d), output(%d x 1)\n", m, n, m);
    for (int j = 0; j < m; j++) {
        y[j] = b[j];
        for (int k = 0; k < n; k++) {
            y[j] += A[j * n + k] * x[k];
        }
    }
}
void relu(int n, const float * x, float * y) {
    if (debugMode == 1) printf("  ReLU    input(%d x 1), output(%d x 1)\n", n, n);
    for (int j = 0; j < n; j++) {
        y[j] = x[j] > 0 ? x[j] : 0;
    }
}

void softmax(int n, const float * x, float * y) {
    if (debugMode == 1) printf("  Softmax input(%d x 1), output(%d x 1)\n", n, n);
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
int inference6(const float * A1, const float * b1, const float * A2, const float * b2, const float * A3, const float * b3, 
                const float * x, float * y) {
    if (debugMode == 1) printf(" Inference6\n");
    float y1[50];
    fc(50, 784, x, A1, b1, y1);
    relu(50, y1, y1);
    float y2[100];
    fc(100, 50, y1, A2, b2, y2);
    relu(100, y2, y2);
    fc(10, 100, y2, A3, b3, y);
    softmax(10, y, y);

    int ym = 0;
    for (int j = 0; j < 10; j++) {
        if (y[j] > y[ym] || j == 0) ym = j;
    }
    return ym;
}

// 誤差逆伝搬(Softmax 層)
void softmaxwithloss_bwd(int n, const float * y, unsigned char t, float * dx) {
    for (int j = 0; j < n; j++) {
        int t_j = t == j ? 1 : 0;
        dx[j] = y[j] - t_j;
    }
}
// 誤差逆伝搬(ReLU 層)
void relu_bwd(int n, const float * x, const float * dy, float * dx) {
    for (int j = 0; j < n; j++) {
        dx[j] = x[j] > 0 ? dy[j] : 0;
    }
}
// 誤差逆伝搬(FC 層)
void fc_bwd(int m, int n, const float * x, const float * dEdy, const float * A,
        float * dEdA, float * dEdb, float * dEdx) {
    for (int j = 0; j < m; j++) {
        for (int k = 0; k < n; k++) {
            dEdA[j * n + k] = dEdy[j] * x[k];
        }
        dEdb[j] = dEdy[j];
    }

    for (int j = 0; j < n; j++) {
        dEdx[j] = 0;
        for (int k = 0; k < m; k++) {
            dEdx[j] += A[k * n + j] * dEdy[k];
        }
    }
}

void backward6(const float * A1, const float * b1, const float * A2, const float * b2, const float * A3, const float * b3, 
        const float * x, unsigned char t, float * y, 
        float * dA1, float * db1, float * dA2, float * db2, float * dA3, float * db3) {
    if (debugMode == 1) printf(" Backward6\n");
    
    float x_relu1[50], x_fc2[50], x_relu2[100], x_fc3[100];
    fc(50, 784, x, A1, b1, x_relu1);
    relu(50, x_relu1, x_fc2);
    fc(100, 50, x_fc2, A2, b2, x_relu2);
    relu(100, x_relu2, x_fc3);
    fc(10, 100, x_fc3, A3, b3, y);
    softmax(10, y, y);

    float y0[784], y1[50], y2[100];
    softmaxwithloss_bwd(10, y, t, y);
    fc_bwd(10, 100, x_fc3, y, A3, dA3, db3, y2);
    relu_bwd(100, x_relu2, y2, y2);
    fc_bwd(100, 50, x_fc2, y2, A2, dA2, db2, y1);
    relu_bwd(50, x_relu1, y1, y1);
    fc_bwd(50, 784, x, y1, A1, dA1, db1, y0);
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

// 学習関連
void updateParam(float *A1, float *b1, float *A2, float *b2, float *A3, float *b3, 
                float *dA1, float *db1, float *dA2, float *db2, float *dA3, float *db3, 
                const int n, const float srate);
// 学習メイン関数
void studyImage(float *A1, float *b1, float *A2, float *b2, float *A3, float *b3, int n, float study_rate) {
    int studyTimes = train_count / n;
    if (debugMode == 1) printf("Study train_count=%d, n=%d, study_rate=%f\n", train_count, n, study_rate);
    
    // (a)
    int *index = malloc(sizeof(int)*train_count);
    for(int k = 0; k < train_count; k++) {
        index[k] = k;
    }
    shuffle(train_count, index);

    // (b)
    float * a_dA1 = malloc(sizeof(float)*784*50);
    float * a_db1 = malloc(sizeof(float)*50);
    float * a_dA2 = malloc(sizeof(float)*50*100);
    float * a_db2 = malloc(sizeof(float)*100);
    float * a_dA3 = malloc(sizeof(float)*10*100);
    float * a_db3 = malloc(sizeof(float)*10);


    float * y = malloc(sizeof(float)*10);
    float * dA1 = malloc(sizeof(float)*784*50);
    float * db1 = malloc(sizeof(float)*50);
    float * dA2 = malloc(sizeof(float)*50*100);
    float * db2 = malloc(sizeof(float)*100);
    float * dA3 = malloc(sizeof(float)*10*100);
    float * db3 = malloc(sizeof(float)*10);
    for (int k = 0; k < studyTimes; k++) {
        init(784 * 50, 0, a_dA1);
        init(50, 0, a_db1);
        init(50 * 100, 0, a_dA2);
        init(100, 0, a_db2);
        init(10 * 100, 0, a_dA3);
        init(10, 0, a_db3);

        for (int l = 0; l < n; l++) {
            backward6(A1, b1, A2, b2, A3, b3, train_x + 784*index[k * n + l], train_y[index[k * n + l]], y, dA1, db1, dA2, db2, dA3, db3);
            add(50 * 784, dA1, a_dA1);
            add(50, db1, a_db1);
            add(100 * 50, dA2, a_dA2);
            add(100, db2, a_db2);
            add(10 * 100, dA3, a_dA3);
            add(10, db3, a_db3);
        }
        updateParam(A1, b1, A2, b2, A3, b3, 
            a_dA1, a_db1, a_dA2, a_db2, a_dA3, a_db3, 
            n, study_rate);
    }

    free(a_dA1); free(a_db1);
    free(a_dA2); free(a_db2);
    free(a_dA3); free(a_db3);
}
// A, bをdA, dbにもとづいてアップデート
void recalc(const int M, const int N, const int n, const float srate, float *A, float *b, float *dA, float *db) {
    divide(M * N, n, dA);
    divide(M, n, db);
    scale(M * N, - srate, dA);
    scale(M, - srate, db);
    add(M * N, dA, A);
    add(M, db, b);
}
void updateParam(float *A1, float *b1, float *A2, float *b2, float *A3, float *b3, 
                float *dA1, float *db1, float *dA2, float *db2, float *dA3, float *db3, 
                const int n, const float srate) {
    recalc(50, 784, n, srate, A1, b1, dA1, db1);
    recalc(100, 50, n, srate, A2, b2, dA2, db2);
    recalc(10, 50, n, srate, A3, b3, dA3, db3);
}

// テスト
void testData(const float *A1, const float *b1, const float *A2, const float *b2, const float *A3, const float *b3, 
                float *start_x, int count, unsigned char *res_y, float *correct, float *l) {
    int sum = 0;
    *l = 0;
    for(int k = 0; k < count; k++) {
        float y[10];
        if(inference6(A1, b1, A2, b2, A3, b3, start_x + k*width*height, y) == res_y[k]) {
            sum++;
        }
        *l += cross_entropy_error(y, res_y[k]);
    }
    
    *l /= 1.0 * count;
    *correct = sum * 100.0 / count;
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

void main_study(int epoc, float study_rate, char *file_prefix) {
    printf("--- Study Mode ---\n");
    clock_t t_start, t_end;
    t_start = clock();

    // 1~3
    int minipatch_n = 100;

    // 4.
    float * A1 = malloc(sizeof(float)*784*50);
    float * b1 = malloc(sizeof(float)*50);
    float * A2 = malloc(sizeof(float)*50*100);
    float * b2 = malloc(sizeof(float)*100);
    float * A3 = malloc(sizeof(float)*10*100);
    float * b3 = malloc(sizeof(float)*10);

    rand_init(784 * 50, A1);
    rand_init(50, b1);
    rand_init(50 * 100, A2);
    rand_init(100, b2);
    rand_init(10 * 100, A3);
    rand_init(10, b3);

    for (int j = 0; j < epoc; j++) {
        printf("[Epoc %d] ", j + 1);

        // (a)(b)
        studyImage(A1, b1, A2, b2, A3, b3, minipatch_n, study_rate);

        // (c) cf)補題7
        if (debugMode >= 1) printf("Test Start\n");
        float testRate, stdRate, avgTestLoss, avgStdLoss;
        testData(A1, b1, A2, b2, A3, b3, test_x, test_count, test_y, &testRate, &avgTestLoss);
        testData(A1, b1, A2, b2, A3, b3, train_x, train_count, train_y, &stdRate, &avgStdLoss);
        if (debugMode >= 1) printf("Test Finished\n");

        printf("Test: %.2f%%(StudyData: %.2f%%), Loss Avg: %f(StudyData: %f)\n", testRate, stdRate, avgTestLoss, avgStdLoss);
        // study_rate *= 0.9;
    }
    t_end = clock();

    printf("[Saving...] ");

    char f1[100], f2[100], f3[100];
    snprintf(f1, 100, "%s%s", file_prefix, "_fc1.dat");
    snprintf(f2, 100, "%s%s", file_prefix, "_fc2.dat");
    snprintf(f3, 100, "%s%s", file_prefix, "_fc3.dat");

    save(f1, 50, 784, A1, b1);
    save(f2, 100, 50, A2, b2);
    save(f3, 10, 100, A3, b3);
    printf("Finished.\n");

    printf("Elapsed[s] = %f\n", (t_end - t_start) * 1.0 / CLOCKS_PER_SEC);
}

void main_inference(char *prefix, float *x) {
    printf("--- Inference Mode ---\n");
    float * A1 = malloc(sizeof(float)*784*50);
    float * b1 = malloc(sizeof(float)*50);
    float * A2 = malloc(sizeof(float)*50*100);
    float * b2 = malloc(sizeof(float)*100);
    float * A3 = malloc(sizeof(float)*10*100);
    float * b3 = malloc(sizeof(float)*10);

    char f1[100], f2[100], f3[100];
    snprintf(f1, 100, "%s%s", prefix, "_fc1.dat");
    snprintf(f2, 100, "%s%s", prefix, "_fc2.dat");
    snprintf(f3, 100, "%s%s", prefix, "_fc3.dat");

    load(f1, 50, 784, A1, b1);
    load(f2, 100, 50, A2, b2);
    load(f3, 10, 100, A3, b3);

    float y[10];
    int res = inference6(A1, b1, A2, b2, A3, b3, x, y);
    printf("Number: %d\n", res);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Input Error!!\nSample: ./main_NN6 study [epoc] [study_rate] [file_prefix]\nSample: ./main_NN6 inf [studydata_prefix] [x]\n");
        return 0;
    }
    srand(time(NULL));

    load_mnist(&train_x, &train_y, &train_count,
                &test_x, &test_y, &test_count,
                &width, &height);

    // これ以降，3層 NN の係数 A_784x10 および b_784x10 と，
    // 訓練データ train_x + 784*i (i=0,...,train_count-1), train_y[0]~train_y[train_count-1], // テストデータ test_x + 784*i (i=0,...,test_count-1), test_y[0]~test_y[test_count-1],
    // を使用することができる.

    char *mode = argv[1];
    if (strcmp(mode, "study") == 0) {
        if (argc != 5) {
            printf("Input Error!!\nSample: ./main_NN6 study [epoc] [study_rate] [file_prefix]\n");
        } else {
            main_study(atoi(argv[2]), atof(argv[3]), argv[4]);
        }
    } else {
        if (argc != 4) {
            printf("Input Error!!\nSample: ./main_NN6 inf [studydata_prefix] [x]\n");
        } else {
            main_inference(argv[2], load_mnist_bmp(argv[3]));
        }
    }

    return 0;
}