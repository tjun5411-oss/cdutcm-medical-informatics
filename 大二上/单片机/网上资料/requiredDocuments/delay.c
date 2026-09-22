#include "delay.h"
#include <intrins.h>

void delay_ms(uint16_t xms) {
    data uint8_t i, j;
    while (xms--) {
        _nop_();
        i = 10;
        j = 100;
        do {
            while (j--);
        } while (i--);
    }
}
