#include <stdio.h>

int main() {
    int n;
    printf("n = ");
    scanf("%d", &n);
    printf("n = %d\n", n);
    printf(" %d (10) = ", n);

    for (int i = 0; i < 32; i++) {
        int h = (n >> (31 - i)) & 1;
        printf("%d", h);
    }
    printf(" (2)\n");
}