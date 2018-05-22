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
void add(int n, const float * x, float * o) {
    for (int j = 0; j < n; j++) {
        o[j] += x[j];
    }
}
int main() {
    float x[6] = {0, 1, 2, 3, 4, 5};
    float y[6] = {1, 1, 2, 3, 5, 8};
    print_oct(2, 3, x, "x");
    print_oct(2, 3, y, "y");
    add(6, x, y);
    print_oct(2, 3, y, "y");
    return 0;
}