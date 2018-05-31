#include <stdio.h>

int main(void) {
    FILE *fp_r = NULL, *fp_w = NULL;
    fp_r = fopen("test.txt", "r");
    if (!fp_r) {
        printf("File cannot open!\n");
        return 0;
    }

    fp_w = fopen("test.dat", "w");
    char str[128];
    while (fgets(str, 128, fp_r)) {
        fputs(str, fp_w);
    }
    fclose(fp_r);
    fclose(fp_w);
}