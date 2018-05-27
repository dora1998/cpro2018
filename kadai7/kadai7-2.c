#include <stdio.h>

int main(void) {
    char str[] = "Sun";
    int n[] = {10, 20, 40};
    double m[] = {1.5, 3.5, 7.5};
    
    printf("--- str (char) ---\n");
    for (int j = 0; j < 3; j++) {
        printf("value: %c, address: %p\n", str[j], &str[j]);
    }

    printf("--- n (int) ---\n");
    for (int j = 0; j < 3; j++) {
        printf("value: %d, address: %p\n", n[j], &n[j]);
    }

    printf("--- m (double) ---\n");
    for (int j = 0; j < 3; j++) {
        printf("value: %lf, address: %p\n", m[j], &m[j]);
    }

    return 0;
}