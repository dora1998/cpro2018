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
void scale(int n, float x, float *o) {
    for (int j = 0; j < n; j++) {
        o[j] *= x;
    }
}
int main() {
    float y[6] = {1, 1, 2, 3, 5, 8};
    print_oct(2, 3, y, "y");
    scale(6, 1.5, y);
    print_oct(2, 3, y, "y");
    return 0;
}