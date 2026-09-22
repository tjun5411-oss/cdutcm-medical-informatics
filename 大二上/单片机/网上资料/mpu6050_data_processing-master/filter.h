
/*
MPU6050 可以输出三轴的加速度和角速度。通过加速度和角速度都可以得到 Pitch 和 Roll 角（加速度不能得到 Yaw 角），
就是说有两组 Pitch、Roll 角，到底应该选哪组呢？别急，先分析一下。MPU6050 的加速度计和陀螺仪各有优缺点，
三轴的加速度值没有累积误差，且通过算 tan()  可以得到倾角，但是它包含的噪声太多（因为待测物运动时会产生加速度，
电机运行时振动会产生加速度等），不能直接使用；陀螺仪对外界振动影响小，精度高，通过对角速度积分可以得到倾角，但是会产生累积误差。
所以，不能单独使用 MPU6050 的加速度计或陀螺仪来得到倾角，需要互补
。一阶互补算法的思想就是给加速度和陀螺仪不同的权值，把它们结合到一起，进行修正
*/

/*
**姿态融合算法：四元数法 、一阶、二阶互补算法、卡尔曼滤波算法。
**四元数一次能输出三个轴的角度，roll，pinch，yaw
**一阶互补滤波一次能输出一个轴的姿态角
**二阶互补滤波一次输出两个轴的姿态角
**卡尔曼滤波一次输出一个轴
*/
#ifndef	__FILTER_H
#define	__FILTER_H

#define PI 3.14

float kalman_filter(float angle_m,float gyro_m); 	//angleAx 和 gyroGy
float one_filter(float angle_m,float gyro_m);//一阶互补滤波
//int get_angle(float x,float y,float z,unsigned char dir)


#endif
