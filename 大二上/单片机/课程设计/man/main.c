#include <reg52.h>
#include "typedefs.h"    // ???????????
#include "uart.h"
#include "syn6288.h"
#include <string.h>

// I2C ???? (??MPU6050)
sbit SDA = P2^0;
sbit SCL = P2^1;

// MPU6050????
#define MPU6050_ADDR 0xD0
#define WHO_AM_I     0x75

// ????
void delay_ms(uint16_t ms);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Write(uint8_t dat);
uint8_t IIC_Read_NAck(void);
uint8_t MPU6050_ReadReg(uint8_t reg);

// ????
void delay_ms(uint16_t ms) {
    uint16_t i,j;
    for(i=ms; i>0; i--)
        for(j=120; j>0; j--);
}

// ... (I2C?MPU6050???????) ...
void IIC_Start(void) {
    SDA = 1; SCL = 1; delay_ms(1);
    SDA = 0; delay_ms(1);
    SCL = 0;
}

void IIC_Stop(void) {
    SDA = 0; SCL = 1; delay_ms(1);
    SDA = 1; delay_ms(1);
}

void IIC_Write(uint8_t dat) {
    uint8_t i;
    for(i=0; i<8; i++) {
        SCL = 0; delay_ms(1);
        SDA = (dat&0x80)>>7;
        dat <<= 1;
        SCL = 1; delay_ms(1);
    }
    SCL = 0; delay_ms(1);
    SDA = 1; delay_ms(1);
    SCL = 1; delay_ms(1);
    SCL = 0;
}

uint8_t IIC_Read_NAck(void) {
    uint8_t i, dat=0;
    SDA = 1; delay_ms(1);
    for(i=0; i<8; i++) {
        SCL = 1; delay_ms(1);
        dat = (dat<<1)|SDA;
        SCL = 0; delay_ms(1);
    }
    SDA = 1; delay_ms(1);
    SCL = 1; delay_ms(1);
    SCL = 0;
    return dat;
}

uint8_t MPU6050_ReadReg(uint8_t reg) {
    uint8_t dat;
    IIC_Start();
    IIC_Write(MPU6050_ADDR);
    IIC_Write(reg);
    IIC_Start();
    IIC_Write(MPU6050_ADDR|0x01);
    dat = IIC_Read_NAck();
    IIC_Stop();
    return dat;
}

void main(void) {
    uart1_init();
    delay_ms(1000);

    syn6288_speak("welcome");
    delay_ms(2000);

    if(MPU6050_ReadReg(WHO_AM_I) == 0x68) {
        syn6288_speak("MPU6050,success");
    } else {
        syn6288_speak("MPU6050,fail");
    }

    while(1);
}