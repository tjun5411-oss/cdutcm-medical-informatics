#include <math.h>
#include "iic.h"
#include "mpu6050.h"
#include "delay.h"
#include "filter.h"

MPU6050_DATA xdata mpu_data = {0};

static void mpu6050_read_acc(void);
static void mpu6050_read_gyro(void);

unsigned char mpu6050_init(void)
{
    if(iic_read_addr_byte(WHO_AM_I) != 0X68)
    {
        return 0;
    }

    iic_write_addr_byte(PWR_MGMT_1, 0x00);
    delay_1ms(1);  
    iic_write_addr_byte(SMPLRT_DIV, 0x00);
    delay_1ms(1); 
    iic_write_addr_byte(CONFIG, 0x06);
    delay_1ms(1); 
    iic_write_addr_byte(GYRO_CONFIG, 0x18);
    delay_1ms(1); 
    iic_write_addr_byte(ACCEL_CONFIG, 0x10);
    return 1;  
}

static void mpu6050_read_acc(void)
{
    unsigned char buff[6] = {0};
    int acc_data[3] = {0};

    iic_continue_read(SlaveAddress, ACC_ADDR, 6, buff);
    acc_data[0] = (int)((buff[0] << 8) | buff[1]);
    acc_data[1] = (int)((buff[2] << 8) | buff[3]);
    acc_data[2] = (int)((buff[4] << 8) | buff[5]);

    mpu_data.acc_data.x = acc_data[0];
    mpu_data.acc_data.y = acc_data[1];
    mpu_data.acc_data.z = acc_data[2];
}

static void mpu6050_read_gyro(void)
{
    unsigned char buff[6] = {0};
    int gyro_data[3] = {0};

    iic_continue_read(SlaveAddress, GYRO_ADDR, 6, buff);
    gyro_data[0] = (int)((buff[0] << 8) | buff[1]);
    gyro_data[1] = (int)((buff[2] << 8) | buff[3]);
    gyro_data[2] = (int)((buff[4] << 8) | buff[5]);

    mpu_data.gyro_data.x = gyro_data[0];
    mpu_data.gyro_data.y = gyro_data[1];
    mpu_data.gyro_data.z = gyro_data[2];
}

// ???????,???????
void mpu6050_read_all(void)
{
    mpu6050_read_acc();
    mpu6050_read_gyro();
}

// ??????(???)
float get_pitch_angle(void)
{
    float ax = (float)mpu_data.acc_data.x / 4096.0;
    float ay = (float)mpu_data.acc_data.y / 4096.0;
    float az = (float)mpu_data.acc_data.z / 4096.0;
    
    return atan2(ax, sqrt(ay*ay + az*az)) * 180.0 / 3.14159;
}

float get_roll_angle(void)
{
    float ax = (float)mpu_data.acc_data.x / 4096.0;
    float ay = (float)mpu_data.acc_data.y / 4096.0;
    float az = (float)mpu_data.acc_data.z / 4096.0;
    
    return atan2(ay, sqrt(ax*ax + az*az)) * 180.0 / 3.14159;
}