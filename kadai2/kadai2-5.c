#include <stdio.h>

int main() {
    for (int j = 0; j < 10; j++) {
        for (int k = j; k < 10; k++) {
            printf("%d", k);
            if (k != 9) printf(" ");
        }
        printf("\n");
    }
    return 0;
}