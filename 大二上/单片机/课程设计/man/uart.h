#ifndef __UART_H
#define __UART_H

#include <reg52.h>
#include "typedefs.h" // ???????????

// ????
void uart1_init();
void uart1_sendByte(uint8_t dat);

#endif