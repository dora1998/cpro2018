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
    if (d != 0) {
        double sd = sqrt(fabs(d));
        if (d > 0) {
            printf("%f\n%f\n", (- b + sd) / (2 * a), (- b - sd) / (2 * a));
        } else {
            double r = - b / (2 * a);
            printf("%f+%fi\n%f-%fi\n", r, sd / (2 * a), r, sd / (2 * a));
        }
    } else {
        printf("%f\n", - b / (2 * a));
    }
    return 0;
}