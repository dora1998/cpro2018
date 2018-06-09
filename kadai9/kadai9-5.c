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

// kadai6-8.c
void init(int n, float x, float *o) {
    for (int j = 0; j < n; j++) {
        o[j] = x;
    }
}

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

int main(int argc, char *argv[]) {
    srand(time(NULL));
    int m = 0, n = 0;
    if (argc > 2) {
        m = atoi(argv[1]);
        n = atoi(argv[2]);
    }
    float *A = malloc(sizeof(float) * m * n);
    float *b = malloc(sizeof(float) * m);
    init(m * n, 1, A);
    init(m, 2, b);
    print(m, n, A);
    print(1, m, b);
    save("test.dat", m, n, A, b);
    return 0;
}