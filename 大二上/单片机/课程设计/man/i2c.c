#include "i2c.h"
#include "delay.h"

// I2C????
void i2c_start(void) {
    SDA = 1;
    SCL = 1;
    delay_us(10);
    SDA = 0;  // SDA????
    delay_us(10);
    SCL = 0;  // ?????,??????
}

// I2C????
void i2c_stop(void) {
    SDA = 0;
    SCL = 1;
    delay_us(10);
    SDA = 1;  // SDA????
    delay_us(10);
}

// ???????
void i2c_send_byte(uint8_t dat) {
    uint8_t i;
    for(i = 0; i < 8; i++) {
        SDA = (dat & 0x80) >> 7;  // ????
        dat <<= 1;
        SCL = 1;                  // ?????,????
        delay_us(10);
        SCL = 0;                  // ?????,?????
        delay_us(10);
    }
}

// ???????
uint8_t i2c_read_byte(void) {
    uint8_t i, dat = 0;
    SDA = 1;  // ????,????
    for(i = 0; i < 8; i++) {
        dat <<= 1;
        SCL = 1;                  // ?????,????
        delay_us(10);
        dat |= SDA;               // ?????
        SCL = 0;                  // ?????
        delay_us(10);
    }
    return dat;
}

// ????
uint8_t i2c_wait_ack(void) {
    uint8_t ack = 1;
    SDA = 1;  // ????
    delay_us(10);
    SCL = 1;  // ?????
    delay_us(10);
    if(SDA == 0) ack = 0;  // ????
    SCL = 0;
    delay_us(10);
    return ack;
}

// ????(0????)
void i2c_ack(void) {
    SDA = 0;
    delay_us(10);
    SCL = 1;
    delay_us(10);
    SCL = 0;
    delay_us(10);
}

// ?????(1?????)
void i2c_nack(void) {
    SDA = 1;
    delay_us(10);
    SCL = 1;
    delay_us(10);
    SCL = 0;
    delay_us(10);
}