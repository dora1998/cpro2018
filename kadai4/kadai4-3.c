#include <stdio.h>
#include <stdlib.h>

/* 関数absを自分で作成 */
float myfabs(float x) {
    if (x < 0) x *= -1;
    return x;
}

int main() {
    float x = -1.3;
    float y = 3.7;
    float abs_x = myfabs(x);
    float abs_y = myfabs(y);
    printf("|x|=%f\n", abs_x);
    printf("|y|=%f\n", abs_y);
    return 0;
}