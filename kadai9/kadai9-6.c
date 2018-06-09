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

int main(int argc, char *argv[]) {
    int m = 0, n = 0;
    if (argc > 2) {
        m = atoi(argv[1]);
        n = atoi(argv[2]);
    }
    float *A = malloc(sizeof(float) * m * n);
    float *b = malloc(sizeof(float) * m);
    load("test.dat", m, n, A, b);
    print(m, n, A);
    print(1, m, b);
    return 0;
}