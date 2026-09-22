#include <stdio.h>

int main() {
    int number, digit, count = 0,i;
    char str[6]; 

    printf("请输入一个五位整数: ");
    scanf("%5s", str);

    number = atoi(str);
    if (number < 10000 || number > 99999) {
        printf("输入的不是一个五位整数！\n");
        return 1; 
    }

    printf("请输入要统计的数字: ");
    scanf("%d", &digit);

    for (i = 0; i < 5; i++) {
        if (str[i] - '0' == digit) { 
            count++;
        }
    }
    printf("数字 %d 在 %s 中出现的次数是: %d\n", digit, str, count);

    return 0;
}
