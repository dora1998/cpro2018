#include <stdio.h>
#include <math.h>

void minmax(int data[], int *tmin, int *tmax) {
    for (int j = 0; j < 3; j++) {
        if (data[j] < *tmin || j == 0) *tmin = data[j];
        if (data[j] > *tmax || j == 0) *tmax = data[j];
    }
}

int main(void) {
    char ordinal[3][4] = {"1st", "2nd", "3rd"};
    int num[3];
    for (int j = 0; j < 3; j++) {
        printf("input %s integer : ", ordinal[j]);
        scanf("%d", &num[j]);
    }

    int tmin, tmax;
    minmax(num, &tmin, &tmax);
    printf("min: %d, max: %d\n", tmin, tmax);
}