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
    int i,min = a < b ? a : b; 
    int divisor = 1; 
 
    for (i = min; i >= 1; i--) {
        if (a % i == 0 && b % i == 0) {
            divisor = i;
            break;
        }
    }

    return divisor;
}
