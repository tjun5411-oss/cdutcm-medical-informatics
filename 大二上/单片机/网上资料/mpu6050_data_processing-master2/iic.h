#ifndef __IIC_H
#define __IIC_H

#include <reg52.h>
#include <intrins.h>

// ???????
extern sbit SCL;
extern sbit SDA;

void iic_init(void);
void iic_start(void);
void iic_stop(void);
void iic_dalay_us(void);
void iic_send_ack(bit _ack);
void iic_wait_ack(void);
void iic_write_byte(unsigned char _byte);
unsigned char iic_read_byte(void);
void iic_write_addr_byte(unsigned char _addr,unsigned char _byte);
unsigned char iic_read_addr_byte(unsigned char _addr);
void iic_continue_read(unsigned char _addr,unsigned char _reg,unsigned char len,unsigned char *buf);

#endif