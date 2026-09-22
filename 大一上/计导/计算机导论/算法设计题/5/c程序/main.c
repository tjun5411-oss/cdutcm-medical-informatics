#include <stdio.h>
int main() {
    int count = 0; 

    int x,y;
    for (x = 1; x * 5 <= 100 - 10 - 20; x++) {


        for ( y = 1; y * 10 <= 100 - 5 * x - 20; y++) { 

            int z = (100 - 5 * x - 10 * y) / 20;

            if (z * 20 == 100 - 5 * x - 10 * y && z >= 1) {
                printf("5元纸币: %d张, 10元纸币: %d张, 20元纸币: %d张\n", x, y, z);
                count++;
            }
        }
    }

    printf("总共有 %d 种换法。\n", count);

    return 0;
}
