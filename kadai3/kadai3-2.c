#include <stdio.h>

int main() {
    int n;
    scanf("%d", &n);
    int res = 1;
    int i = 1;
    while (i < n) {
        i++;
        res *= i;
    }
    printf("%d\n", res);
}