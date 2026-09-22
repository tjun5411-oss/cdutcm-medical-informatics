#ifndef __MPU6050_H
#define __MPU6050_H

#define SMPLRT_DIV      0x19
#define CONFIG          0x1A
#define GYRO_CONFIG     0x1B
#define ACCEL_CONFIG    0x1C
#define ACC_ADDR        0X3B
#define ACCEL_XOUT_H    0x3B
#define ACCEL_XOUT_L    0x3C
#define ACCEL_YOUT_H    0x3D
#define ACCEL_YOUT_L    0x3E
#define ACCEL_ZOUT_H    0x3F
#define ACCEL_ZOUT_L    0x40
#define GYRO_ADDR       0X43
#define GYRO_XOUT_H     0x43
#define GYRO_XOUT_L     0x44
#define GYRO_YOUT_H     0x45
#define GYRO_YOUT_L     0x46
#define GYRO_ZOUT_H     0x47
#define GYRO_ZOUT_L     0x48
#define PWR_MGMT_1      0x6B
#define WHO_AM_I        0x75
#define SlaveAddress    0xD0

typedef struct _int
{
    int x;
    int y;
    int z;
    int temp_out;
} data_int;

typedef struct _mpu6050
{
    data_int acc_data;
    data_int gyro_data;
} MPU6050_DATA;

extern MPU6050_DATA mpu_data;

unsigned char mpu6050_init(void);
void mpu6050_read_all(void);
float get_pitch_angle(void);
float get_roll_angle(void);

#endif