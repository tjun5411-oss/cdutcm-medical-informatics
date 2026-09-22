#include "iic.h"
#include "mpu6050.h"

// ????????????
sbit SCL = P1^5;  // ????SCL??
sbit SDA = P1^4;  // ????SDA??

void iic_dalay_us(void)
{
    unsigned char i;
    _nop_();
    i = 1;
    while(--i);
}

void iic_start(void)
{
    SDA = 1;
    iic_dalay_us();
    SCL = 1;
    iic_dalay_us();
    SDA = 0;
    iic_dalay_us();
    SCL = 0;
}

void iic_stop(void)
{
    SDA = 0;
    iic_dalay_us();
    SCL = 0;
    iic_dalay_us();
    SCL = 1;
    iic_dalay_us();
    SDA = 1;
    iic_dalay_us();
}

void iic_init(void)
{
    SCL = 1;
    iic_dalay_us();
    SDA = 1;	
    iic_dalay_us();
}

void iic_send_ack(bit _ack)
{
    SDA = _ack;
    SCL = 1;
    iic_dalay_us();
    SCL = 0;
    iic_dalay_us();
}

void iic_wait_ack(void)
{
    unsigned char i = 0;
    SCL = 1;
    iic_dalay_us();
    while((SDA==1)&&(i<250))i++;
    SCL = 0;
}

void iic_write_byte(unsigned char _byte)
{
    unsigned char i;
    for(i=0;i<8;i++)
    {
        SDA = _byte&0X80;
        iic_dalay_us();
        SCL = 1;
        iic_dalay_us();
        SCL = 0;
        iic_dalay_us();
        _byte = _byte<<1;	
    }
    SDA = 1;
    iic_wait_ack();
}

void iic_write_addr_byte(unsigned char _addr,unsigned char _byte)
{
    iic_start();
    iic_write_byte(SlaveAddress);
    iic_write_byte(_addr);
    iic_write_byte(_byte);	
    iic_stop();
}

unsigned char iic_read_byte(void)
{
    unsigned char i = 0;
    unsigned char read_byte = 0;
    SDA = 1;
    iic_dalay_us();
    for(i = 0;i<8;i++)
    {
        SCL = 1;
        read_byte = (read_byte<<1)|SDA;
        iic_dalay_us();
        SCL = 0;
        iic_dalay_us();
    }
    return read_byte;
}

unsigned char iic_read_addr_byte(unsigned char _addr)
{
    unsigned char read_data;
    iic_start();
    iic_write_byte(SlaveAddress);
    iic_write_byte(_addr);
    iic_start();
    iic_write_byte(SlaveAddress + 1);
    read_data = iic_read_byte();
    iic_send_ack(1);
    iic_stop();
    return read_data;
}

void iic_continue_read(unsigned char _addr,unsigned char _reg,unsigned char len,unsigned char *buf)
{
    iic_start();
    iic_write_byte(_addr);
    iic_write_byte(_reg);
    iic_start();
    iic_write_byte(_addr + 1);
    while(len)
    {
        *buf++ = iic_read_byte();
        if(len == 1)
            iic_send_ack(1);
        else
            iic_send_ack(0);
        len--;
    }
    iic_stop();
}