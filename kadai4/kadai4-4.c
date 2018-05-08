#include <stdio.h>

#define M 3
#define N 4

int a[M][N];

void input(int i, int j, int val);
void output(int i, int j);

int main() {
    int i, j;
    for(i = 0; i < M; i++) {
        for(j = 0; j < N; j++) {
           input(i, j, 10 * i + j);
           output(i, j);
        }
    }

    return 0;
}

void input(int i, int j, int val) {
    a[i][j] = val;
}
void output(int i, int j) {
    printf("%2d", a[i][j]);
    if (j == (N - 1)) {
        printf("\n");
    } else {
        printf(" ");
    }
}