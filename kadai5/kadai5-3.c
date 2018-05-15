#include <stdio.h>

char a;
void cprint() {
    printf("%c %d %x\n", a, a, a);
}
int main() {
    a = 'a';
    cprint();
    a++;
    cprint();
    return 0;
}