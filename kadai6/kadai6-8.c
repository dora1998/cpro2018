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
void init(int n, float x, float *o) {
    for (int j = 0; j < n; j++) {
        o[j] = x;
    }
}
void add(int n, const float * x, float * o) {
    for (int j = 0; j < n; j++) {
        o[j] += x[j];
    }
}
void mul(int m, int n, const float *x, const float *A, float *o) {
    for (int j = 0; j < m; j++) {
        o[j] = 0;
        for (int k = 0; k < n; k++) {
            o[j] += A[j * n + k] * x[k];
        }
    }
}
void fc(int m, int n, const float *x, const float *A, const float *b, float *o) {
    mul(m, n, x, A, o);
    add(m, b, o);
}
int main() {
    float A[6] = {1,2,3,4,5,6};
    float b[2] = {0.5, 0.25};
    float x[3] = {2,3,5};
    float o[2];
    fc(2, 3, x, A, b, o);
    print_oct(2, 3, A, "A");
    print_oct(2, 1, b, "b");
    print_oct(3, 1, x, "x");
    print_oct(2, 1, o, "o");
    return 0;
}