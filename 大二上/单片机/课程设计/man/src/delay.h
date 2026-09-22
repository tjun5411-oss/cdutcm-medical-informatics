#ifndef __DELAY_H
#define __DELAY_H

#include "typedef.h"

void delay_init(void);
void delay_us(uint16_t us);  // ?????(STC89C52RC????,??=10us)
void delay_ms(uint16_t ms);  // ?????

#endif