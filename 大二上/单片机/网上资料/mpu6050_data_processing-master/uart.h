#ifndef __UART_H
#define __UART_H

#include <reg52.h>

// ???(??,???????)
#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

// ??????(extern ??,?????)
extern void uart_init(void);  // ?????
extern void uart_send_char(unsigned char dat);  // ??????
extern void uart_send_byte(unsigned char dat);  // ???????
extern void uart_send_string(unsigned char *str);  // ?????(????)
extern void uart_send_senser(void);  // ???????

#endif