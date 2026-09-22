
#include <reg52.h>
#include <intrins.h>
#include <stdio.h>	//配置完串口后，使用这个头文件里面的printf
#include "iic.h"
#include "mpu6050.h"
#include "delay.h"
#include "uart.h"


void main()
{	 
	delay_500ms(2);	 //待单片机上电稳定之后初始化。前人踩过的坑还是小心点好

	uart_init();
	iic_init();
	delay_1ms(100);  

	while(!(mpu6050_init()));		//等待6050初始化成功返回1；

	while(1)
	{

		mpu6050_data_process();
		uart_send_senser();
	 		
	}

}



