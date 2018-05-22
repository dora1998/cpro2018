#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
void rand_init(int n, float *o) {
    for (int j = 0; j < n; j++) {
        o[j] = rand() / (1.0 + RAND_MAX) * 2.0 - 1.0;
    }
}
int main() {
    srand(time(NULL));
    float y[6];
    // 初期化前の値
    print_oct(2, 3, y, "y");
    rand_init(6, y);
    // 初期化後の値
    print_oct(2, 3, y, "y");
    return 0;
}