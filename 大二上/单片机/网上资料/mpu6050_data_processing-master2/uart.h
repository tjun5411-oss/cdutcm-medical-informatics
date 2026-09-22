
#ifndef __UART_H
#define	__UART_H

/**********为了匿名四轴上位机的协议定义的变量****************************/
//cup为小端模式存储，也就是在存储的时候，低位被存在0字节，高位在1字节
#define BYTE0(dwTemp)       (*(char *)(&dwTemp))	 //取出int型变量的低字节
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))	 //	取存储在此变量下一内存字节的内容，高字节
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))


void uart_init();  //串口初始化
void uart_send_char(unsigned char _data);	 //发送一个字符
void uart_send_string(unsigned char *_data,unsigned char cnt);	//发送字符串
void uart_send_senser(void);   //6050数据打包按照上位机协议发送给上位机

#endif

