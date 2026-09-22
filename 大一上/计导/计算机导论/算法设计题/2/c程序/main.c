#include <stdio.h>

int main() {
    int n;
    for (n = 5000; n < 10000; n++) {
        if (n % 5 == 1 && n % 6 == 5 && n % 7 == 4 && n % 11 == 10) {
            printf("一共有 %d 士兵\n", n);
            break;
        }
    }

    return 0;
}
