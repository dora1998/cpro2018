#include <stdio.h>

int c(int n, int r) {
    if (r == 0 || r == n) return 1;
    if (r == 1) return n;

    return c(n - 1, r - 1) + c(n - 1, r);
}

int main() {
    int n, r;
    scanf("%d%d", &n, &r);
    printf("%d\n", c(n, r));
    return 0;
}