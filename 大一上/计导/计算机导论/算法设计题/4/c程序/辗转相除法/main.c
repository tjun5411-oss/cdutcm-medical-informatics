#include <stdio.h>
int gcd(int a, int b);

int main() {
    int num1, num2, result;

    printf("请输入两个正整数: ");
    scanf("%d %d", &num1, &num2);

    result = gcd(num1, num2);

    printf("%d 和 %d 的最大公约数是: %d\n", num1, num2, result);

    return 0;
}

int gcd(int a, int b) {
    while (b != 0) { 
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}
