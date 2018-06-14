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

// kadai6-6.c
void rand_init(int n, float *o) {
    for (int j = 0; j < n; j++) {
        o[j] = rand() * 1.0 / RAND_MAX * 2.0 - 1.0;
    }
}

void relu(int n, const float * x, float * y) {
    for (int j = 0; j < n; j++) {
        y[j] = x[j] > 0 ? x[j] : 0;
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
    relu(n, y, y);
    print(1, n, y);
    return 0;
}