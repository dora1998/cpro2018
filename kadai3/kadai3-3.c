#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GU 0
#define CHOKI 2
#define PA 5

int getComHand() {
    int hands[3] = {GU, CHOKI, PA};
    return hands[rand() % 3];
}

int main() {
    srand(time(NULL));
    int res = 0;
    while (res == 0) {
        int user;
        printf("Your input (0,2,5): ");
        scanf("%d", &user);
        if (!(user == GU || user == CHOKI || user == PA)) {
            printf("Invalid input => Input again.\n");
            continue;
        }

        int com = getComHand();
        if (user == com) {
            printf("Comp:%d vs You:%d => Try again.\n", com, user);
        } else {
            res = user < com ? 1 : -1;
            if ((user + com) == (GU + PA)) res *= -1;
            if (res == 1) {
                printf("Comp:%d vs You:%d => You win.\n", com, user);
            } else {
                printf("Comp:%d vs You:%d => You lose.\n", com, user);
            }
        }
    }
}