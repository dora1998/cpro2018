#include <stdio.h>
#include "nn.h"

void print(int m, int n, const float *x) {
    for (int j = 0; j < m; j++) {
        for (int k = 0; k < n; k++) {
            printf("%.4f", x[j * m + k]);
            if (k != (n - 1)) printf(" ");
        }
        printf("\n");
    }
}
int main(void) {
    print(1, 10, b_784x10);
    return 0;
}