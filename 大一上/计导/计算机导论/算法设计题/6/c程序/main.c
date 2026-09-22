#include <stdio.h>

int main() {
    int ducks = 2; 
    int villages = 7; 
    int sold[villages];
    int i,before; 
    for ( i = villages - 1; i >= 0; i--) {
        before = (ducks + 1) * 2;
        sold[i] = before / 2 - ducks; 
        ducks = before; 
    }

    printf("出发时共赶了 %d 只鸭子。\n", ducks);
    printf("经过每个村子卖出的鸭子数量分别为：\n");
    for ( i = 0; i < villages; i++) {
        printf("第%d个村子: %d 只\n", i + 1, sold[i]);
    }

    return 0;
}
