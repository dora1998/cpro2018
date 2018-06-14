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
void swap(float *x, float *y) {
    float t = *y;
    *y = *x;
    *x = t;
}
void sort(float *data, int n) {
    for (int j = 0; j < (n - 1); j++) {
        for (int k = 0; k < (n - j - 1); k++) {
            if (data[k] > data[k + 1]) swap(&data[k], &data[k + 1]);
        }
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
    return 0;
}