#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <omp.h>
#include "nn.h"
#include "nn_stdlib.h"

const int debugMode = 0;

float *train_x = NULL;
unsigned char *train_y = NULL;
int train_count = -1;
float *test_x = NULL;
unsigned char *test_y = NULL;
int test_count = -1;
int width = -1;
int height = -1;

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

// 学習関連
void updateParam(float *A1, float *b1, float *A2, float *b2, float *A3, float *b3, 
                float *dA1, float *db1, float *dA2, float *db2, float *dA3, float *db3, 
                const int n, const float srate);
/* 学習メイン関数
    A1~A3, b1~b3: 学習対象行列
    n: ミニパッチ数
    study_rate: 学習率
*/
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

    for (int k = 0; k < studyTimes; k++) {
        init(784 * 50, 0, a_dA1);
        init(50, 0, a_db1);
        init(50 * 100, 0, a_dA2);
        init(100, 0, a_db2);
        init(10 * 100, 0, a_dA3);
        init(10, 0, a_db3);

        #pragma omp parallel for
        for (int l = 0; l < n; l++) {
            float * dA1 = malloc(sizeof(float)*784*50);
            float * db1 = malloc(sizeof(float)*50);
            float * dA2 = malloc(sizeof(float)*50*100);
            float * db2 = malloc(sizeof(float)*100);
            float * dA3 = malloc(sizeof(float)*10*100);
            float * db3 = malloc(sizeof(float)*10);
            float * y = malloc(sizeof(float)*10);

            backward6(A1, b1, A2, b2, A3, b3, train_x + 784*index[k * n + l], train_y[index[k * n + l]], y, dA1, db1, dA2, db2, dA3, db3);

            #pragma omp critical
            {
                add(50 * 784, dA1, a_dA1);
                add(50, db1, a_db1);
                add(100 * 50, dA2, a_dA2);
                add(100, db2, a_db2);
                add(10 * 100, dA3, a_dA3);
                add(10, db3, a_db3);
            }
            
            free(dA1); free(db1);
            free(dA2); free(db2);
            free(dA3); free(db3);
            free(y);
        }
        updateParam(A1, b1, A2, b2, A3, b3, 
            a_dA1, a_db1, a_dA2, a_db2, a_dA3, a_db3, 
            n, study_rate);
        printProgressBar(k * 80.0 / studyTimes);
    }

    free(a_dA1);
    free(a_db1);
    free(a_dA2);
    free(a_db2);
    free(a_dA3);
    free(a_db3);
}

/* A, bをdA, dbにもとづいてアップデートする関数
    num: 何段階目か(0~)
    M, N: Aの行列サイズ
    n: ミニパッチ数
    srate: 学習率
    A, b: 更新先配列
    dA, db: 更新元データ配列
*/
// SGD
void recalc_SGD(const int num, const int M, const int N, const int n, const float srate, float *A, float *b, float *dA, float *db) {
    divide(M * N, n, dA);
    divide(M, n, db);
    scale(M * N, - srate, dA);
    scale(M, - srate, db);
    add(M * N, dA, A);
    add(M, db, b);
}
// Momentum
float *vA[] = {NULL, NULL, NULL};
float *vb[] = {NULL, NULL, NULL};
const float alpha = 0.9;
void recalc_Momentum(const int num, const int M, const int N, const int n, const float srate, float *A, float *b, float *dA, float *db) {
    if (vA[num] == NULL) {
        vA[num] = malloc(sizeof(float) * M * N);
        vb[num] = malloc(sizeof(float) * M);
        init(M * N, 0, vA[num]);
        init(M, 0, vb[num]);
    } else {
        scale(M * N, alpha, vA[num]);
        scale(M, alpha, vb[num]);
    }

    divide(M * N, n, dA);
    divide(M, n, db);
    scale(M * N, - srate, dA);
    scale(M, - srate, db);
    add(M * N, dA, vA[num]);
    add(M, db, vb[num]);

    add(M * N, vA[num], A);
    add(M, vb[num], b);
}
// AdaGrad
float *hA[] = {NULL, NULL, NULL};
float *hb[] = {NULL, NULL, NULL};
void recalc_AdaGrad(const int num, const int M, const int N, const int n, const float srate, float *A, float *b, float *dA, float *db) {
    if (hA[num] == NULL) {
        hA[num] = malloc(sizeof(float) * M * N);
        hb[num] = malloc(sizeof(float) * M);
        init(M * N, 0, hA[num]);
        init(M, 0, hb[num]);
    }

    divide(M * N, n, dA);
    divide(M, n, db);

    float *tmp_dA = malloc(sizeof(float) * M * N);
    float *tmp_db = malloc(sizeof(float) * M);
    acopy(M * N, dA, tmp_dA);
    acopy(M, db, tmp_db);
    matpow(M * N, tmp_dA);
    matpow(M, tmp_db);
    add(M * N, tmp_dA, hA[num]);
    add(M, tmp_db, hb[num]);

    int j = 0, k = 0;
    #pragma omp parallel for private(k)
    for (j = 0; j < M; j++) {
        for (k = 0; k < N; k++) {
            dA[j * N + k] *= - srate / (sqrt(hA[num][j * N + k]) + 1e-7);
        }
        db[j] *= - srate / (sqrt(hb[num][j]) + 1e-7);
    }
    
    add(M * N, dA, A);
    add(M, db, b);

    free(tmp_dA); free(tmp_db);
}

/* A1~A3, b1~b3をまとめてアップデートする関数
    n: ミニパッチ数
    srate: 学習率
    A1~A3, b1~b3: 更新先配列
    dA1~dA3, db1~db3: 更新元データ配列
*/
void updateParam(float *A1, float *b1, float *A2, float *b2, float *A3, float *b3, 
                float *dA1, float *db1, float *dA2, float *db2, float *dA3, float *db3, 
                const int n, const float srate) {
    #pragma omp parallel sections
    {
        #pragma omp section
        recalc_AdaGrad(0, 50, 784, n, srate, A1, b1, dA1, db1);
        #pragma omp section
        recalc_AdaGrad(1, 100, 50, n, srate, A2, b2, dA2, db2);
        #pragma omp section
        recalc_AdaGrad(2, 10, 50, n, srate, A3, b3, dA3, db3);
    }
}

/* テスト関数
    A1~A3, b1~b3: テストに使用する行列
    start_x: テストデータの開始位置ポインタ
    count: テストデータ数
    res_y: テストデータの正解データ配列
    correct: 正解率返却用ポインタ
    l: ロス平均返却用ポインタ
*/
void testData(const float *A1, const float *b1, const float *A2, const float *b2, const float *A3, const float *b3, 
                float *start_x, int count, unsigned char *res_y, float *correct, float *l) {
    int sum = 0, k = 0;
    float loss = 0;
    #pragma omp parallel for reduction(+:loss) reduction(+:sum)
    for(k = 0; k < count; k++) {
        float y[10];
        if(inference6(A1, b1, A2, b2, A3, b3, start_x + k*width*height, y) == res_y[k]) {
            sum++;
        }
        loss += cross_entropy_error(y, res_y[k]);
    }
    
    loss /= 1.0 * count;
    *l = loss;
    *correct = sum * 100.0 / count;
}

/* 学習モードのメイン関数
    epoc: エポック回数
    study_rate: 学習率
    file_prefix: 保存先ファイルプレフィックス
*/
void main_study(int epoc, float study_rate, char *file_prefix) {
    char logmes[100];
    initLog(file_prefix);
    snprintf(logmes, 100, "%s%d", "Epoc\t", epoc);
    writeLog(logmes);
    snprintf(logmes, 100, "%s%f", "Study Rate\t", study_rate);
    writeLog(logmes);
    printf("--- Study Mode ---\n");

    struct timespec t_start, t_end;
    clock_gettime(CLOCK_REALTIME, &t_start);

    // 1~3
    int minipatch_n = 100;

    // 4.
    float * A1 = malloc(sizeof(float)*784*50);
    float * b1 = malloc(sizeof(float)*50);
    float * A2 = malloc(sizeof(float)*50*100);
    float * b2 = malloc(sizeof(float)*100);
    float * A3 = malloc(sizeof(float)*10*100);
    float * b3 = malloc(sizeof(float)*10);

    rand_init_bm(784 * 50, A1, 784);
    rand_init_bm(50, b1, 784);
    rand_init_bm(50 * 100, A2, 50);
    rand_init_bm(100, b2, 50);
    rand_init_bm(10 * 100, A3, 100);
    rand_init_bm(10, b3, 100);

    writeLog("\nEpoc\tRate(Test)\tRate(Study)\tLoss(Test)\tLoss(Study)");
    for (int j = 0; j < epoc; j++) {
        printProgressBar(0.0);
        // (a)(b)
        studyImage(A1, b1, A2, b2, A3, b3, minipatch_n, study_rate);
        printProgressBar(80.0);

        // (c) cf)補題7
        if (debugMode >= 1) printf("Test Start\n");
        float testRate, stdRate, avgTestLoss, avgStdLoss;
        #pragma omp parallel sections
        {
            #pragma omp section
            testData(A1, b1, A2, b2, A3, b3, test_x, test_count, test_y, &testRate, &avgTestLoss);
            #pragma omp section
            testData(A1, b1, A2, b2, A3, b3, train_x, train_count, train_y, &stdRate, &avgStdLoss);
        }
        printProgressBar(100.0);
        eraseProgressBar();
        if (debugMode >= 1) printf("Test Finished\n");

        printf("[Epoc %d] ", j + 1);
        printf("Test: %.2f%%(StudyData: %.2f%%), Loss Avg: %f(StudyData: %f)\n", testRate, stdRate, avgTestLoss, avgStdLoss);
        snprintf(logmes, 100, "%d\t%f\t%f\t%f\t%f", j + 1, testRate, stdRate, avgTestLoss, avgStdLoss);
        writeLog(logmes);
        
        study_rate *= 0.9;
    }
    clock_gettime(CLOCK_REALTIME, &t_end);
    long elapsed_sec, elapsed_nsec;
    if (t_end.tv_nsec < t_start.tv_nsec) {
        elapsed_sec = t_end.tv_sec - t_start.tv_sec - 1;
        elapsed_nsec = t_end.tv_nsec + (long int)1.0e+9 - t_start.tv_nsec;
    } else {
        elapsed_sec = t_end.tv_sec - t_start.tv_sec;
        elapsed_nsec = t_end.tv_nsec - t_start.tv_nsec;
    }
    printf("Elapsed[s] = %5ld.%06ld\n", elapsed_sec, elapsed_nsec);
    snprintf(logmes, 100, "%s%5ld.%06ld", "Elapsed[s] = ", elapsed_sec, elapsed_nsec);
    writeLog(logmes);

    printf("[Saving...] ");

    char f1[100], f2[100], f3[100];
    snprintf(f1, 100, "%s%s", file_prefix, "_fc1.dat");
    snprintf(f2, 100, "%s%s", file_prefix, "_fc2.dat");
    snprintf(f3, 100, "%s%s", file_prefix, "_fc3.dat");

    save(f1, 50, 784, A1, b1);
    save(f2, 100, 50, A2, b2);
    save(f3, 10, 100, A3, b3);
    printf("Finished.\n");

    writeLogToFile();
}

/* 推論モードメイン関数
    prefix: 読込ファイルプレフィックス
    x: 推論対象ファイル
*/
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