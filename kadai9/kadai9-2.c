#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// kadai8-1.c
void print(float *data, int n) {
    for (int j = 0; j < n; j++) {
        printf("%.4f ", data[j]);
    }
    printf("\n");
}

// kadai6-6.c
void rand_init(int n, float *o) {
    for (int j = 0; j < n; j++) {
        o[j] = rand() * 1.0 / RAND_MAX * 2.0 - 1.0;
    }
}

// kadai8-1.c
void fswap(float *x, float *y) {
    float t = *y;
    *y = *x;
    *x = t;
}
void iswap(int *x, int *y) {
    int t = *y;
    *y = *x;
    *x = t;
}
void sort(float *data, int n) {
    for (int j = 0; j < (n - 1); j++) {
        for (int k = 0; k < (n - j - 1); k++) {
            if (data[k] > data[k + 1]) fswap(&data[k], &data[k + 1]);
        }
    }
}

int irand(const int max, const int base) {
    return (int)(rand() / (1.0 + RAND_MAX) * (max - base) + base);
}
void shuffle(int n, int *x) {
    for (int j = 0; j < n; j++) {
        int k = irand(n, 0);
        iswap(&x[j], &x[k]);
    }
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    int n = 0;
    if (argc > 1) n = atoi(argv[1]);
    float *y = malloc(sizeof(float) * n);
    rand_init(n, y);
    print(y, n);
    sort(y, n);
    print(y, n);

    int *index = malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) index[i] = i;
    shuffle(n, index);

    for (int i = 0; i < n; i++) printf("%.4f ", y[index[i]]);
    printf("\n");
    return 0;
}