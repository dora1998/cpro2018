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
int main(void) {
    float m[12] = {0, 1, 2, 3, 4, 5,
        6, 7, 8, 9, 10, 11};
    print_oct(3, 4, m, "m");
    return 0;
}