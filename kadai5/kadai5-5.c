#include <stdio.h>

int main() {
    float a = 0;
    float b = 0;
    double c = 0;
    double d = 0;

    for (int j = 0; j < 1e+8; j++) {
        a += 1e-8;
        c += 1e-8;
    }

    b = 1e-8 * 1e+8;
    d = 1e-8 * 1e+8;

    printf("a = %.20f, b = %.20f, c = %.20f, d = %.20f\n", a, b, c, d);
    return 0;
}