#include <stdio.h>

int main(void) {
    FILE *file;
    file = fopen("test2.txt", "w");
    fprintf(file, "Hello, World!");
    fclose(file);
    return 0;
}