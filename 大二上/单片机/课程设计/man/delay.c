#include "delay.h"
#include <intrins.h>  // ??_nop_()??

// ?????(STC89C52RC????????,???????)
void delay_init(void) {
    // ???,STC89C52RC?????
}

// ?????(11.0592MHz??,?10us~65535us)
void delay_us(uint16_t us) {
    uint16_t i;
    while(us--) {
        _nop_();_nop_();_nop_();_nop_();
        _nop_();_nop_();_nop_();_nop_();
        _nop_();_nop_();_nop_();_nop_();  // ?1us@11.0592MHz
    }
}

// ?????(11.0592MHz??)
void delay_ms(uint16_t ms) {
    uint16_t i, j;
    for(i = 0; i < ms; i++) {
        for(j = 0; j < 112; j++);  // ?1ms@11.0592MHz
    }
}