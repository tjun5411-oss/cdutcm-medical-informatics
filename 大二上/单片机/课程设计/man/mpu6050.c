#include "mpu6050.h"
#include "i2c.h"
#include "delay.h"

// ?MPU6050?????
static void mpu6050_write_reg(uint8_t reg, uint8_t dat) {
    i2c_start();
    i2c_send_byte(MPU6050_ADDR);    // ??????(?)
    if(i2c_wait_ack()) {            // ????
        i2c_stop();
        return;
    }
    i2c_send_byte(reg);             // ???????
    i2c_wait_ack();
    i2c_send_byte(dat);             // ????
    i2c_wait_ack();
    i2c_stop();
}

// ?MPU6050?????
static uint8_t mpu6050_read_reg(uint8_t reg) {
    uint8_t dat;
    i2c_start();
    i2c_send_byte(MPU6050_ADDR);    // ??????(?)
    i2c_wait_ack();
    i2c_send_byte(reg);             // ???????
    i2c_wait_ack();
    
    i2c_start();
    i2c_send_byte(MPU6050_ADDR | 0x01);  // ??????(?)
    i2c_wait_ack();
    dat = i2c_read_byte();               // ????
    i2c_nack();                          // ?????
    i2c_stop();
    return dat;
}

// ???MPU6050
uint8_t mpu6050_init(void) {
    uint8_t id;
    delay_ms(100);                     // ????
    
    mpu6050_write_reg(PWR_MGMT_1, 0x00);  // ??MPU6050(????)
    delay_ms(10);
    id = mpu6050_read_reg(WHO_AM_I);      // ????ID
    if(id != 0x68) return 1;              // ID????1
    
    mpu6050_write_reg(SMPLRT_DIV, 0x07);  // ?????(1kHz)
    mpu6050_write_reg(ACCEL_CONFIG, 0x08); // ??????±4g
    mpu6050_write_reg(GYRO_CONFIG, 0x08);  // ?????±500°/s
    return 0;                             // ???????0
}

// ????????
void mpu6050_read_accel(float *ax, float *ay, float *az) {
    uint8_t buf[6];
    int16_t accel_x, accel_y, accel_z;
    
    // ????6????(?ACCEL_XOUT_H??)
    i2c_start();
    i2c_send_byte(MPU6050_ADDR);
    i2c_wait_ack();
    i2c_send_byte(ACCEL_XOUT_H);
    i2c_wait_ack();
    
    i2c_start();
    i2c_send_byte(MPU6050_ADDR | 0x01);
    i2c_wait_ack();
    buf[0] = i2c_read_byte(); i2c_ack();  // ACCEL_XOUT_H
    buf[1] = i2c_read_byte(); i2c_ack();  // ACCEL_XOUT_L
    buf[2] = i2c_read_byte(); i2c_ack();  // ACCEL_YOUT_H
    buf[3] = i2c_read_byte(); i2c_ack();  // ACCEL_YOUT_L
    buf[4] = i2c_read_byte(); i2c_ack();  // ACCEL_ZOUT_H
    buf[5] = i2c_read_byte(); i2c_nack(); // ACCEL_ZOUT_L
    i2c_stop();
    
    // ??16???????g(±4g??16384 LSB/g)
    accel_x = (buf[0] << 8) | buf[1];
    accel_y = (buf[2] << 8) | buf[3];
    accel_z = (buf[4] << 8) | buf[5];
    *ax = (float)accel_x / 16384.0f;
    *ay = (float)accel_y / 16384.0f;
    *az = (float)accel_z / 16384.0f;
}

// ???????
void mpu6050_read_gyro(float *gx, float *gy, float *gz) {
    uint8_t buf[6];
    int16_t gyro_x, gyro_y, gyro_z;
    
    // ????6????(?GYRO_XOUT_H??)
    i2c_start();
    i2c_send_byte(MPU6050_ADDR);
    i2c_wait_ack();
    i2c_send_byte(GYRO_XOUT_H);
    i2c_wait_ack();
    
    i2c_start();
    i2c_send_byte(MPU6050_ADDR | 0x01);
    i2c_wait_ack();
    buf[0] = i2c_read_byte(); i2c_ack();  // GYRO_XOUT_H
    buf[1] = i2c_read_byte(); i2c_ack();  // GYRO_XOUT_L
    buf[2] = i2c_read_byte(); i2c_ack();  // GYRO_YOUT_H
    buf[3] = i2c_read_byte(); i2c_ack();  // GYRO_YOUT_L
    buf[4] = i2c_read_byte(); i2c_ack();  // GYRO_ZOUT_H
    buf[5] = i2c_read_byte(); i2c_nack(); // GYRO_ZOUT_L
    i2c_stop();
    
    // ??16???????°/s(±500°/s??65.5 LSB/(°/s))
    gyro_x = (buf[0] << 8) | buf[1];
    gyro_y = (buf[2] << 8) | buf[3];
    gyro_z = (buf[4] << 8) | buf[5];
    *gx = (float)gyro_x / 65.5f;
    *gy = (float)gyro_y / 65.5f;
    *gz = (float)gyro_z / 65.5f;
}