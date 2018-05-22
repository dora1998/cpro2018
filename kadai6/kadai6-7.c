#include <stdio.h>

void print_oct(int m, int n, const float *x, const char *name) {
    printf("%s = [", name);
    for (int j = 0; j < m; j++) {
        for (int k = 0; k < n; k++) {
            printf(" ");
            printf("%.4f", x[j * n + k]);
        }
        printf(";\n");
    }
    printf("];\n");
}
void mul(int m, int n, const float *x, const float *A, float *o) {
    for (int j = 0; j < m; j++) {
        o[j] = 0;
        for (int k = 0; k < n; k++) {
            o[j] += A[j * n + k] * x[k];
        }
    }
}
int main() {
    float A[6] = {1,2,3,4,5,6};
    float x[3] = {2,3,5};
    float o[2];
    mul(2, 3, x, A, o);
    print_oct(2, 3, A, "A");
    print_oct(3, 1, x, "x");
    print_oct(2, 1, o, "o");
    return 0;
}