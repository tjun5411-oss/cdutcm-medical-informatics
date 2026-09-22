
#include "filter.h"
#include "IIC.h"
#include "mpu6050.h"
#include <math.h>

float xdata one_filter_angle = 0;
float xdata kalman_filter_angle = 0, kalman_filter_angle_dot = 0;

//----------------
//在进行滤波之前，需要获取原始数据 
float one_filter(float angle_m,float gyro_m)
{

	float xdata K1 =0.1; // 对加速度计取值的权重
	float xdata dt=0.005;//注意：dt的取值为滤波器采样时间

	one_filter_angle = K1 * angle_m+ (1-K1) * (one_filter_angle + gyro_m * dt);
	return one_filter_angle;
}


//------------------
float kalman_filter(float angle_m,float gyro_m)
{

	//滤波参数
	float xdata dt = 0.005;   //卡尔曼采样时间
	float xdata P[2][2]	= {{1,0},{0,1}};
	float xdata Pdot[4]	= {0,0,0,0};
	float xdata Q_angle = 0.001;//角度数据置信度,陀螺仪协方差
	float xdata Q_gyro = 0.005;	 //角速度数据置信度，陀螺仪飘移噪声协方差
	float xdata R_angle = 0.5;	//加速度计协方差
	char xdata	C_0 = 1;
	float xdata q_bias = 0,angle_err = 0; //q_bias为陀螺仪飘移
	float xdata PCt_0 = 0,PCt_1 = 0,E = 0;
	float xdata K_0 = 0,  K_1 = 0,  t_0 = 0,  t_1 = 0;


 	kalman_filter_angle+= (gyro_m - q_bias) * dt;	//卡尔曼预测方程，认为每次飘移相同，


    Pdot[0]=Q_angle - P[0][1] - P[1][0];
    Pdot[1]=- P[1][1];
    Pdot[2]=- P[1][1];
    Pdot[3]=Q_gyro;

    P[0][0] += Pdot[0] * dt;
    P[0][1] += Pdot[1] * dt;
    P[1][0] += Pdot[2] * dt;
    P[1][1] += Pdot[3] * dt;

    PCt_0 = C_0 * P[0][0];	 //矩阵乘法中间变量
    PCt_1 = C_0 * P[1][0];

    E = R_angle + C_0 * PCt_0;	 //分母

    K_0 = PCt_0 / E;   //增益值
    K_1 = PCt_1 / E;

	angle_err = angle_m - kalman_filter_angle;	
    kalman_filter_angle += K_0 * angle_err; //对状态的卡尔曼估计，最优角度
    q_bias += K_1 * angle_err;
    kalman_filter_angle_dot = gyro_m-q_bias;//最优角速度

	t_0 = PCt_0;	 //矩阵计算中间变量
    t_1 = C_0 * P[0][1];

    P[0][0] -= K_0 * t_0;
    P[0][1] -= K_0 * t_1;
    P[1][0] -= K_1 * t_0;
    P[1][1] -= K_1 * t_1;

    return kalman_filter_angle;
}


/*
**原子程序
**dir---->0:与z轴的角度，1：与自然x轴的角度，2：与自然y轴的角度。
**输入：沿三个轴的重力加速度分量,无单位
**仅仅通过加速度计值计算角度，包含的噪声太多
*/
/*
int get_angle(float x,float y,float z,unsigned char dir)
{
	float xdata temp = 0;
	float xdata res = 0;

	switch(dir)
	{
		case 0:	   //0:与z轴的角度
			temp = sqrt((x*x + y*y)) / z;
			res = atan(temp);
			break;
		case 1:	   //1：与自然x轴的角度
			temp = x / sqrt((y*y + z*z));
			res = atan(temp);
			break;
		case 2:	   //2：与自然y轴的角度
			temp = y / sqrt((x*x + z*z));
			res = atan(temp);
			break;
	}

	return (int)(res * 1800 / 3.14) ;
}
*/			   



