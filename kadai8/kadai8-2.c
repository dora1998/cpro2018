#include <stdio.h>
#include <math.h>

typedef struct {
    double x, y;
} Vector2d;

double GetLength(Vector2d *t) {
    return sqrt(pow(t->x, 2) + pow(t->y, 2));
}

int main(void) {
    Vector2d v;
    printf("Input 2D Vector: ");
    scanf("%lf%lf", &v.x, &v.y);
    printf("Length: %lf\n", GetLength(&v));
}