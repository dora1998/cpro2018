#include <stdio.h>

int input();
int fact(int n);
int perm(int n, int r);

int main() {
   int n = input();
   for (int j = 0; j <= n; j++) {
       int p = perm(n, j);
       printf("perm(%d,%d) = %d\n", n, j, p);
   }
}

int input() {
    int r = 0;
    int n;
    while (r != 1) {
        printf("n = ");
        r = scanf("%d", &n);
        if (r != 1) scanf("%*s");

        if (n < 0) r = 0;
        if (r != 1) printf("Invalid input\n");
    }
    return n;
}

int fact(int n) {
   int res = 1;
   for (int j = 2; j <= n; j++) {
       res *= j;
   }
   return res;
}

int perm(int n, int r) {
    if (r == 0) return 1;
    return fact(n) / fact(n - r);
}