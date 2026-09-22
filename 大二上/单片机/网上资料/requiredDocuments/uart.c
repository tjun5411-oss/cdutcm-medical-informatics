#include "uart.h"
#include "stc12c5a32s2.h"

//9600bps@11.0592MHz
void uart1_init() {
    PCON &= 0x7F;		//波特率不倍速
    SCON = 0x50;		//8位数据,可变波特率
    AUXR &= 0xBF;		//定时器1时钟为Fosc/12,即12T
    AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
    TMOD &= 0x0F;		//清除定时器1模式位
    TMOD |= 0x20;		//设定定时器1为8位自动重装方式
    TL1 = 0xFD;		//设定定时初值
    TH1 = 0xFD;		//设定定时器重装值
    ET1 = 0;		//禁止定时器1中断
    REN = 1;
    TR1 = 1;		//启动定时器1
    EA = 1;
    ES = 1;
}

//9600bps@11.0592MHz
void uart2_init() {
    AUXR &= 0xF7;		//波特率不倍速
    S2CON = 0x50;		//8位数据,可变波特率
    AUXR &= 0xFB;		//独立波特率发生器时钟为Fosc/12,即12T
    BRT = 0xFD;			//设定独立波特率发生器重装值
    AUXR |= 0x10;		//启动独立波特率发生器
}

void uart1_sendByte(uint8_t dat) {
    SBUF = dat;
    while (TI == 0);
    TI = 0;
}

void uart2_sendByte(uint8_t dat) {
    S2BUF = dat;
    while ((S2CON & S2TI) == 0);
    S2CON &= ~S2TI;
}
