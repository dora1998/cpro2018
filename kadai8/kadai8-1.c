#include <stdio.h>

void print(int *data, int n) {
    for (int j = 0; j < n; j++) {
        printf("%d ", data[j]);
    }
    printf("\n");
}
void swap(int *x, int *y) {
    int t = *y;
    *y = *x;
    *x = t;
}

void bsort(int *data, int n) {
    for (int j = 0; j < (n - 1); j++) {
        if (data[j] > data[j + 1]) swap(&data[j], &data[j + 1]);
    }
}

int main(void) {
    int n = 6;

    int data[] = {64, 30, 8, 87, 45, 13};
    printf("Data: ");
    print(data, n);
    printf("N: %d\n\n", n);

    for (int j = 0; j < (n - 1); j++) {
        bsort(data, n - j);
        printf("loop%d: ", j + 1);
        print(data, n);
    }
}