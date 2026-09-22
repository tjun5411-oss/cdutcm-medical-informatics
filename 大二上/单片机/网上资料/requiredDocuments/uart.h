#ifndef __UART_H
#define __UART_H

#include "typedef.h"

#define S2RI  0x01          //S2CON.0
#define S2TI  0x02          //S2CON.1
#define S2RB8 0x04          //S2CON.2
#define S2TB8 0x08          //S2CON.3

// 串口 1 初始化
void uart1_init();
// 串口 1 发送字节数据
void uart1_sendByte(uint8_t dat);
// 串口 2 初始化
void uart2_init();
// 串口 2 发送字节数据
void uart2_sendByte(uint8_t dat);

#endif 