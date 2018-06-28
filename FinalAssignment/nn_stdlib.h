#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "nn.h"
#include "nn_calclib.h"

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