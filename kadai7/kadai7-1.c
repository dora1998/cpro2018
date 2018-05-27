#include <stdio.h>

int main(void) {
    char s[1000];
    printf("Input a word: ");
    scanf("%s", s);

    int j = 0;
    while(s[j]) {
        if (s[j] == 'z' || s[j] == 'Z') {
            s[j] -= 'z' - 'a';
        } else {
            s[j] += 1;
        }
        j++;
    }
    printf("%s\n", s);
}