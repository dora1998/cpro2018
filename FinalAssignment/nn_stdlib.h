#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "nn.h"
#include "nn_calclib.h"

#define LOG_BUFFER_SIZE 10000   //ログのバッファサイズ

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

// ログ記録バッファ
char logbuf[LOG_BUFFER_SIZE];
char logfname[100];
// ログファイルへのバッファ書込
void writeLogToFile() {
    FILE *fpp;

    fpp = fopen(logfname, "a");
    if( fpp == NULL )
    {
        printf("Error: Can't save logfile\n");
        return;
    }
    
    fputs(logbuf, fpp);
    fclose(fpp);
}
// ログ追加関数(mes: ログメッセージ)
void writeLog(const char *mes) {
    if ((strlen(logbuf) + strlen(mes) + 1) >= LOG_BUFFER_SIZE) writeLogToFile();
    strcat(logbuf, mes);
    strcat(logbuf, "\n");
}
// ログ日時情報記録関数(fileprefix: ファイル接頭辞)
void initLog(const char *fileprefix) {
    snprintf(logfname, sizeof(logfname), "%s%s", fileprefix, ".log");

    char date_str[256];
    time_t timer = time(NULL);
    strftime(date_str, sizeof(date_str), "%x %H:%M:%S", localtime(&timer));
}

const char rotateChar[] = {'/', '-', '\\', '|', '-', '\\', '|'};
const int ROTATECHAR_LENGTH = 7;
int progressing = -1;
// プログレスバーの表示(progress: 進捗(0~100))
void printProgressBar(float progress) {
    if (progressing == -1) {
        progressing = 0;
    } else {
        progressing = (progressing + 1) % ROTATECHAR_LENGTH;
    }

    int pmax = progress / 5;
    printf("[%-20.*s] %c %.1f%%\r", pmax, "********************", rotateChar[progressing], progress);
}
// プログレスバーの消去
void eraseProgressBar() {
    if (progressing == -1) return;
    printf("\033[2K\033[G");
    progressing = -1;
}