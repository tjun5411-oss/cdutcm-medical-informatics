
#ifndef __IIC_H
#define	__IIC_H

#include <reg52.h>
#include <intrins.h>

sbit SCL = P2^0;
sbit SDA = P2^1;

void iic_init(void);
void iic_start(void);
void iic_stop(void);
void iic_dalay_us(void);//iic延时专用
void iic_send_ack(bit _ack);//读取数据之后发送应答位
void iic_wait_ack(void);//写入数据之后，等待从机应答信号
void iic_write_byte(unsigned char _byte); //写一个字节
unsigned char iic_read_byte(void);//读取一个字节
void iic_write_addr_byte(unsigned char _addr,unsigned char _byte);//任意地址写一个字节
unsigned char iic_read_addr_byte(unsigned char _addr); //任意地址读一个字节
void iic_continue_read(unsigned char _addr,unsigned char _reg,unsigned char len,unsigned char *buf);	//地址连续读取，在6050中用，节省时间



#endif


