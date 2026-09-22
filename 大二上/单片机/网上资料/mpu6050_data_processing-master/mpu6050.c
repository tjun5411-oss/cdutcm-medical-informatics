#include <math.h>
#include "iic.h"
#include "mpu6050.h"
#include "delay.h"
#include "filter.h"

MPU6050_DATA xdata mpu_data = {0};  //存放6050运算数据，刚开始最好手动初始化


/*
**如果函数只在本文件中被调用，最好在本源文件中声明为静态函数。
**好处：1、避免函数重名问题。
		2、静态函数会被分配在一个被一直使用的存储区，直到退出程序，避免了调用函数时的压栈出栈，速度加快
*/
static void mpu6050_read_acc(void);	 //读取原始值
static void mpu6050_read_gyro(void);
static void cal_acc_offset(int* acc_data);//计算加速度计补偿值，零偏
static void cal_gyro_offset(int* gyro_data);

//------------初始化,返回初始化是否成功---------------
unsigned char mpu6050_init(void)
{  
	if(iic_read_addr_byte(WHO_AM_I) != 0X68) //检查6050是否正常
	{
	   return 0;
	}

    iic_write_addr_byte(PWR_MGMT_1, 0x00);  //电源管理，解除休眠状态
	delay_1ms(1);  
    iic_write_addr_byte(SMPLRT_DIV, 0x00);  //陀螺仪采样率，典型值0x00，陀螺仪输出频率为8KHZ,
	delay_1ms(1); 
    iic_write_addr_byte(CONFIG, 0x06);   //配置加速度计和陀螺仪的滤波器，加速度计始终1KHZ输出率，滤波的话。陀螺仪1KHZ输出，通过datasheet可以看到，滤波就是滤掉了250hz的频率分量
    delay_1ms(1); 
	iic_write_addr_byte(GYRO_CONFIG, 0x18);  //陀螺仪自检及测量范围，典型值0x18，不自检，2000dps ,LSB:16.4
    delay_1ms(1); 
	iic_write_addr_byte(ACCEL_CONFIG, 0x10);  //加速度计自检、测量范围，典型值0x01，(不自检，+-2G，5Hz) ,65535/4 = 16384 LSB
											  //如果+-2G的话，波形显示波动单位太大，0X1F,选择量程+-16G, 65535/32 = 2048 LSB
											  //0X10是 +-8G,    4096LSB
	return 1;  

}

//------读取三轴加速度计的原始值，16位二进制数------
static void mpu6050_read_acc(void)	//16位adc
{
	unsigned char buff[6] = {0};
	int acc_data[3] = {0};
	unsigned char i = 0;

//	for(i = 0;i<6;i++)
//	{
//		buff[i] = iic_read_addr_byte(ACC_ADDR + i);
//	}
	iic_continue_read(SlaveAddress, ACC_ADDR, 6, buff);	//地址连续读取，在6050中用，节省时间

	acc_data[0] = (int)((buff[0] << 8) | buff[1]);//x轴高8位左移8位或上低8位
	acc_data[1] = (int)((buff[2] << 8) | buff[3]);
	acc_data[2] = (int)((buff[4] << 8) | buff[5]);

	cal_acc_offset(acc_data);

	mpu_data.acc_data.x = (acc_data[0] - mpu_data.acc_offset.x);
	mpu_data.acc_data.y = (acc_data[1] - mpu_data.acc_offset.y);
	mpu_data.acc_data.z = (acc_data[2] - mpu_data.acc_offset.z);
//	mpu_data.acc_data.z = acc_data[2];

}

//------读取三轴陀螺仪原始值-----
//注意陀螺仪的零偏现象，与温度和器件有关。
static void mpu6050_read_gyro(void)
{
	unsigned char buff[6] = {0};
	int gyro_data[3] = {0};
	unsigned char i = 0;

//	for(i = 0;i<6;i++)	//重复读取6次比较慢，可以修改iic函数，一次读取六个数据
//	{
//		buff[i] = iic_read_addr_byte(GYRO_ADDR + i);
//	}

	iic_continue_read(SlaveAddress, GYRO_ADDR, 6, buff);
		
	gyro_data[0] = (int)((buff[0] << 8) | buff[1]);//x轴高8位左移8位或上低8位
	gyro_data[1] = (int)((buff[2] << 8) | buff[3]);
	gyro_data[2] = (int)((buff[4] << 8) | buff[5]);

	cal_gyro_offset(gyro_data);//传入原始数据，计算零偏，

	mpu_data.gyro_data.x = gyro_data[0] - mpu_data.gyro_offset.x + 3000;
	mpu_data.gyro_data.y = gyro_data[1] - mpu_data.gyro_offset.y ;
	mpu_data.gyro_data.z = gyro_data[2] - mpu_data.gyro_offset.z ;
	
}

/*
**计算加速度计补偿值，就是刚开始的零偏，原始值多次累加，求平均作为零度偏移量。
**以后获取的加速度的值都需要减去这个偏移量。
**一开始这里想用循环一直累加多次，但是细想这样不行，如果在这里面用循环一直累加的话，累加的其实都是一个量。
**需要在主循环里面，每获取一次加速度的值累加一次，这样才是有效的数据。
**2017.12.11-----//一定要注意自己定义的数据类型和这个变量的取值范围,因为这个小问题没注意到浪费了很多时间
*/
static void cal_acc_offset(int* acc_data)
{
	if(ACC_OFFSET_FLAG)
	{
		static unsigned int i = 0;	   //原来问题处在这里，一开始定义i为unsigned char，这个最大表示256，而下面我却写1000，									
		static long int xdata acc_x_temp = 0, acc_y_temp = 0, acc_z_temp = 0;
		
		if(i == 0) //每个周期清零一次
		{
			acc_x_temp = 0;
			acc_y_temp = 0; 
			acc_z_temp = 0;
		}
	
		acc_x_temp+= acc_data[0];
		acc_y_temp+= acc_data[1];
		acc_z_temp+= acc_data[2];
		i++;
	
		if(i == 500)
		{
			mpu_data.acc_offset.x = acc_x_temp/(float)i;
			mpu_data.acc_offset.y = acc_y_temp/(float)i;
			mpu_data.acc_offset.z = acc_z_temp/(float)i;
	
			i = 0;
			return;	
		}
	}
}
/*
//陀螺仪零偏校准
//这种方法会使一开始的数据不准确，因为前200个数据没有被校准，我想能不能让他一开始就进行校准，
//就是刚开始获取几个数就取前几个数的平均，只是刚开始不是很精确，但是到了一定的程度，比如已经累加到2000个数据了，校准已经
//精确了，这时，停止更新 mpu_data.gyro_offset 
*/

static void cal_gyro_offset(int* gyro_data)
{
	if(GYRO_OFFSET_FLAG)
	{
		static unsigned int i = 0;	 
		static long int xdata gyro_x_temp = 0, gyro_y_temp = 0, gyro_z_temp = 0;
		
		if(i == 0) 
		{
			gyro_x_temp = 0;
			gyro_y_temp = 0; 
			gyro_z_temp = 0;
		}
	
		gyro_x_temp+= gyro_data[0];
		gyro_y_temp+= gyro_data[1];
		gyro_z_temp+= gyro_data[2];
		i++;
	
		if(i == 500)		//数据产生一个校准值，然后重新计算校准值
		{
			mpu_data.gyro_offset.x = gyro_x_temp/(float)i;
			mpu_data.gyro_offset.y = gyro_y_temp/(float)i;
			mpu_data.gyro_offset.z = gyro_z_temp/(float)i;
	
			i = 0;
			return;	
		}
	}
}

/*
**2017.12.5
**对6050的原始数据进行处理，滤波，角度转换。
**滑动平均滤波
*/


void mpu6050_data_process(void)
{

	unsigned char i = 0,  j = 0;
	static unsigned char acc_filter_cnt = 0;//滤波次数计数
	static unsigned char gyro_filter_cnt = 0;
	long int xdata acc_temp[3] = {0};//三轴加速度,注意，这个不能定义为静态
	long int xdata gyro_temp[3] = {0};
	static int  acc_x_buf[ACC_FILTER_NUM] = {0}, //滑动窗口缓存，每调用一次，里面数据多一个。
	    	    acc_y_buf[ACC_FILTER_NUM] = {0},
	    	    acc_z_buf[ACC_FILTER_NUM] = {0};

	static int  gyro_x_buf[GYRO_FILTER_NUM] = {0}, //滑动窗口缓存，每调用一次，里面数据多一个。
   			    gyro_y_buf[GYRO_FILTER_NUM] = {0},
   	            gyro_z_buf[GYRO_FILTER_NUM] = {0};

	float xdata init_ax = 0,init_ay = 0,init_az = 0;
	float xdata acc_angle_x = 0,gyro_angle_x = 0;	//一阶互补的参数传递


	mpu6050_read_acc();	 //获取原始数据
	mpu6050_read_gyro();

	init_ax =  (float)mpu_data.acc_data.x / ACC_G;	   //计算单位为G的各轴重力加速度分量
	init_ay =  (float)mpu_data.acc_data.y / ACC_G;
	init_az =  (float)mpu_data.acc_data.z / ACC_G;

	acc_angle_x = atan(init_ax/init_az) * 180 / PI ;	//加速度计x轴角度
	gyro_angle_x = -(float)mpu_data.gyro_data.y / 7510.0;	 //陀螺仪计算x轴角度,7150??

	mpu_data.acc_angle.x = (int)one_filter(acc_angle_x,gyro_angle_x);		 //一阶互补滤波，获取偏x轴角度
	mpu_data.acc_angle.y = (int)kalman_filter(acc_angle_x,gyro_angle_x);     //获取卡尔曼滤波


//加速度计数据处理
//-----------------------------------------------------
	acc_x_buf[acc_filter_cnt] = mpu_data.acc_data.x;//平均滤波 ,更新数据，一开始这里写成了加，数据一直不对
	acc_y_buf[acc_filter_cnt] = mpu_data.acc_data.y;
	acc_z_buf[acc_filter_cnt] = mpu_data.acc_data.z;

	for(i=0;i<ACC_FILTER_NUM;i++)	//滑动滤波,取平均值
	{
		acc_temp[0]+= acc_x_buf[i];
		acc_temp[1]+= acc_y_buf[i];
		acc_temp[2]+= acc_z_buf[i];
	}

	//在此进行 六面校准
	//---------------------------
	mpu_data.acc_filter.x = (acc_temp[0] / ACC_FILTER_NUM); //得出沿x轴方向的重力加速度， 单位G
	mpu_data.acc_filter.y = (acc_temp[1] / ACC_FILTER_NUM);
	mpu_data.acc_filter.z = (acc_temp[2] / ACC_FILTER_NUM);
	//--------------------------		  

	acc_filter_cnt++;
	if(acc_filter_cnt == ACC_FILTER_NUM)
	{
		acc_filter_cnt = 0;
	}

//陀螺仪数据处理
//-----------------------------------------------------------
	gyro_x_buf[gyro_filter_cnt] = mpu_data.gyro_data.x;
	gyro_y_buf[gyro_filter_cnt] = mpu_data.gyro_data.y;
	gyro_z_buf[gyro_filter_cnt] = mpu_data.gyro_data.z;

	for(j=0;j<GYRO_FILTER_NUM;j++)	//滑动滤波,取平均值
	{
		gyro_temp[0]+= gyro_x_buf[j];
		gyro_temp[1]+= gyro_y_buf[j];
		gyro_temp[2]+= gyro_z_buf[j];
	}

	//在此进行 六面校准
	//-----------------------------
	mpu_data.gyro_filter.x = gyro_temp[0]/GYRO_FILTER_NUM;
	mpu_data.gyro_filter.y = gyro_temp[1]/GYRO_FILTER_NUM;
	mpu_data.gyro_filter.z = gyro_temp[2]/GYRO_FILTER_NUM;
	//--------------------------

	gyro_filter_cnt++;
	if(gyro_filter_cnt == GYRO_FILTER_NUM)
	{
		gyro_filter_cnt = 0;
	}

	//化为度制
//	mpu_data.gyro_dps.x = (float)mpu_data.gyro_filter.x * GYRO_A;
//	mpu_data.gyro_dps.y = (float)mpu_data.gyro_filter.y * GYRO_A;
//	mpu_data.gyro_dps.z = (float)mpu_data.gyro_filter.z * GYRO_A;
//
//	//化为弧度制
//	mpu_data.gyro_rad.x = (float)mpu_data.gyro_filter.x * GYRO_R;
//	mpu_data.gyro_rad.y = (float)mpu_data.gyro_filter.y * GYRO_R;
//	mpu_data.gyro_rad.z = (float)mpu_data.gyro_filter.z * GYRO_R;


}


