#include <stdio.h>
#include <math.h>

int main()
{
    double a, b, c;
    printf("a = ");
    scanf("%lf", &a);
    printf("b = ");
    scanf("%lf", &b);
    printf("c = ");
    scanf("%lf", &c);

    double d = pow(b, 2) - 4 * a * c;
    double other = - b / (2 * a);
    if (d != 0) {
        d = sqrt(d);
        printf("%f\n%f\n", other + d / (2 * a), other - d / (2 * a));
    } else {
        printf("%f\n", other);
    }
    return 0;
}