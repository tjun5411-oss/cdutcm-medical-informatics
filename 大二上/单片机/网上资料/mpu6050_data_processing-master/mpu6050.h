
/*
**为了保证数据的物理意义，MPU6050的加速度计是以假想球在三轴上座标值的相反数作为三个轴的加速度值。
**当假想球的位置偏向一个轴的正向时，该轴的加速度读数为负值，当假想球的位置偏向一个轴的负向时，该轴的加速度读数为正值。
**例如，当加速度计的量程设置为+-2g，那么当ACC取最小值-32768时，意味着当前加速度方向为沿x轴正方向2倍的重力加速度。
*/
#ifndef __MPU6050_H
#define __MPU6050_H

// 定义MPU6050内部地址  
//****************************************  
#define SMPLRT_DIV      0x19    //陀螺仪采样率，典型值：0x07(125Hz)  
#define CONFIG          0x1A    //低通滤波频率，典型值：0x06(5Hz)  
#define GYRO_CONFIG     0x1B    //陀螺仪自检及测量范围，典型值：0x18(不自检，+-2000deg/s)，65535/4000 = 16.4LSB  
#define ACCEL_CONFIG    0x1C    //加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，+-2G，5Hz) ,65535/4 = 16384LSB

#define ACC_ADDR        0X3B 	//作为首地址，方便后面读取
#define ACCEL_XOUT_H    0x3B  
#define ACCEL_XOUT_L    0x3C  
#define ACCEL_YOUT_H    0x3D  
#define ACCEL_YOUT_L    0x3E  
#define ACCEL_ZOUT_H    0x3F  
#define ACCEL_ZOUT_L    0x40 
 
#define TEMP_OUT_H      0x41  	//温度，T(du) = 36.53 + temp_out/340
#define TEMP_OUT_L      0x42

#define GYRO_ADDR       0X43  	 //作为首地址，方便后面读取
#define GYRO_XOUT_H     0x43  
#define GYRO_XOUT_L     0x44      
#define GYRO_YOUT_H     0x45  
#define GYRO_YOUT_L     0x46  
#define GYRO_ZOUT_H     0x47  
#define GYRO_ZOUT_L     0x48 
 
#define PWR_MGMT_1      0x6B    //电源管理，典型值：0x00(正常启用)  
#define WHO_AM_I        0x75    //IIC地址寄存器(默认数值0x68，只读)  
#define SlaveAddress    0xD0    //IIC写入时的地址字节数据，+1为读取 

/*单位换算*/
#define RtoA 57.29577952f		//弧度到度 180/PI
#define AtoR 0.01745329f		//度到弧度 1/180*PI
#define GYRO_A  0.060975609f	//角速度化为度制		1/16.4 			注：GYRO量程+-2000度有效
#define GYRO_R  0.001064225f	//角速度化为弧度制      1/16.4*0.01745329f

//滑动平均滤波数据个数
#define ACC_FILTER_NUM 6
#define GYRO_FILTER_NUM 3

//这个值与量程有关，我选择的+-8G量程，32768/8 = 4096，这个数是当前量程下6050的灵敏度，4096LSB/G
//以后获取的ADC值，直接除这个数就是真实的单位为G的加速度了，
#define ACC_G  4096
#define ACC_2G 2048 

//并不是一直进行零偏校准的，如果一直校准的话，数据就失去其意义了。
#define ACC_OFFSET_FLAG 0
#define GYRO_OFFSET_FLAG 0
 
//6050数据定义
typedef struct _int
{
	int x;
	int y;
	int z;
	int tepm_out;
}data_int;

typedef struct _float
{
	float x;
	float y;
	float z;
	float temp_out;
}data_float;

//需要用到的结构体
typedef struct _mpu6050
{
	data_int acc_data;
	data_int gyro_data;
	data_int acc_offset;
	data_int gyro_offset;

	data_int acc_filter;
	data_int gyro_filter;

	data_int acc_angle;//由加速度计计算得来的角度值

//	data_float gyro_dps;
//	data_float gyro_rad;

}xdata MPU6050_DATA; 

//6050操作函数
unsigned char mpu6050_init(void);
void mpu6050_data_process(void);


extern MPU6050_DATA mpu_data;
extern float kalman_x;


#endif 


