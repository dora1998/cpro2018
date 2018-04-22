#include <stdio.h>

int main(){
    int x;
    int sum=0;

    for(x=0; x<100; x++) {
        if ((x % 3) != 0) sum += x;
    }
    printf("sum=%d\n", sum);
    return 0;
}