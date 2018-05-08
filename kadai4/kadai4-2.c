#include <stdio.h>

#define M 3
#define N 4

int main() {
    int a[M][N];
    int i, j;

    for(i = 0; i < M; i++) {
        for(j = 0; j < N; j++) {
           a[i][j] = 10 * i + j;
           printf("%2d", a[i][j]);
           if (j != (N - 1)) printf(" ");
        }
        printf("\n");
    }

    return 0;
}