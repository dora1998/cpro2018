#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* 1~10の乱数を最適な方法で生成 */
int lrand() {
    return (int)(rand() / (1.0 + RAND_MAX) * 10.0);
}

/* 関数histを自分で作成 */
void hist(int n) {
    // 回数配列の初期化
    int r[10];
    for (int j = 0; j < 10; j++) {
        r[j] = 0;
    }

    // n回の試行
    for (int j = 0; j < n; j++) {
        int p = lrand();
        r[p] = r[p] + 1;
    }

    printf("n=%7d:", n);
    // ％の算出 & 出力
    for (int j = 0; j < 10; j++) {
        float rp = r[j] * 100.0 / n;
        printf(" %4.1f", rp);   // "4"は小数点や小数部も含めた全文字数であることに注意
    }
    printf("\n");
}

int main() {
    srand(time(NULL));
    hist(100);
    hist(1000);
    hist(10000);
    hist(100000);
    hist(1000000);
    return 0;
}