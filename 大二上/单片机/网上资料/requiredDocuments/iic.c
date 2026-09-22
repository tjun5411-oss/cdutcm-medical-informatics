
//可以用keil自带的逻辑分析仪查看iic端口的输出波形，通过对波形的分析，判断iic协议有没有工作。
#include "iic.h"
#include "mpu6050.h"


//---------5us,实际测试是8us--------
void iic_dalay_us(void)
{
	unsigned char i;

		_nop_();//nop是1个指令周期 = 1机器周期 = 12时钟周期 = 12*1/f，对于12M晶振，1us
		i = 1;
		while(--i);		//执行一次while循环2us
}


//-------起始信号，时钟高，数据高变低---------------
void iic_start(void)
{
	SDA = 1;
	iic_dalay_us();
	SCL = 1;
	iic_dalay_us();
	SDA = 0;
	iic_dalay_us();
	SCL = 0;       //钳住I2C总线，准备发送或接收数据
}

//--------停止信号-------
void iic_stop(void)
{
	SDA = 0;
	iic_dalay_us();
	SCL = 0;
	iic_dalay_us();
	SCL = 1;
	iic_dalay_us();
	SDA = 1;
	iic_dalay_us();
}

//-------------
void iic_init(void)
{
	SCL = 1;
	iic_dalay_us();
	SDA = 1;	
	iic_dalay_us();
}


//----读取数据完成后，发送应答位，
//----0->ack，应答，告诉从机我要继续读取下一个字节，从机收到这个信号后继续发送数据
//----1->not ack，不应答，告诉从机我不在继续接受数据，从机停止发送数据
void iic_send_ack(bit _ack)
{
	SDA = _ack;//接收完成后，拉高SDA,发送非应答信号
	SCL = 1;
	iic_dalay_us();
	SCL = 0; //拉低，完成应答位
	iic_dalay_us();
}


//-------------主机发送完成，等待应答----------------
//主机发送完一个字节的数据之后会把SDA拉高，等待从机的信号，如果从机不应答，SDA将一直拉高，此时while函数等待250us，
//如果在SCL高电平期间，SDA被从设备拉低表示从设备应答，此时while跳过，应答位结束，
void iic_wait_ack(void)
{
	unsigned char i = 0;

	SCL = 1;
	iic_dalay_us();
	while((SDA==1)&&(i<250))i++;//应答时，SDA为0表示从机成功接受到数据，
	SCL = 0;   //拉低，完成应答位
}

//------------------------
//写一个字节，从高位往低位写
//主机给从机发送数据，从机是在时钟的下降沿采集SDA的数据
void iic_write_byte(unsigned char _byte) 
{
	unsigned char i;
	
	for(i=0;i<8;i++)
	{
		SDA = _byte&0X80;	 //先写高位
		iic_dalay_us();
		SCL = 1;
		iic_dalay_us();
		SCL = 0;		  //下降沿采集数据
		iic_dalay_us();
		_byte = _byte<<1;	
	}

	SDA = 1;//发送完毕后，释放数据线，检测从机应答
	iic_wait_ack();//等待应答
}


//----------------------------------
//任意地址写一个字节，因为我使用的时候从设备只有一个，所以不需要写入从设备地址，
//如果有多个从设备的话，可以增加一个参数，写入从设备地址
void iic_write_addr_byte(unsigned char _addr,unsigned char _byte)
{
	iic_start();
	iic_write_byte(SlaveAddress); //此处写入从机地址，我这里只有一个，没有在函数里面体现
	iic_write_byte(_addr);
	iic_write_byte(_byte);	
	iic_stop();
}

//------------------------
//读字节,也是先读取的是高位，需要左移
//主机从从机读取数据，本质上是从机检测主机发出的时钟信号有8个上升沿
unsigned char iic_read_byte(void)
{
	unsigned char i = 0;
	unsigned char read_byte = 0;

	SDA = 1;	  //先确保主机释放SDA
	iic_dalay_us();

	for(i = 0;i<8;i++)
	{
		SCL = 1;
		read_byte = (read_byte<<1)|SDA ;
		iic_dalay_us();
		SCL = 0;	//拉低，采集数据
		iic_dalay_us();
	}

	return read_byte;
}

//------------------
//在某一地址内读取数据
unsigned char iic_read_addr_byte(unsigned char _addr)
{
	unsigned char read_data;

	iic_start();
	iic_write_byte(SlaveAddress);//发送设备地址+写信号
	iic_write_byte(_addr);	//发送存储器单元
	iic_start();			//必须从新启动IIC
	iic_write_byte(SlaveAddress + 1);//发送设备地址+读信号
	read_data = iic_read_byte();//读出数据
	iic_send_ack(1);//发送非应答信号
	/*
	原来问题出在这里！！，如果后面跟的是iic_stop的话，就发送非应答，告诉从机不要在发数据了，注意这里没有连续读取
	不然的话，从机收到0的应答位之后会一直发送数据，但是这时主机又没有接受，导致数据只能第一次发送成功，后面都收不到数据。
	*/
	iic_stop();

	return read_data;
}

//----------------------
//为节省时间，连续读取
//_addr：从机地址
//_reg：从机需要读取的寄存器地址

void iic_continue_read(unsigned char _addr,unsigned char _reg,unsigned char len,unsigned char *buf)
{
	iic_start();
	iic_write_byte(_addr);
	iic_write_byte(_reg);
	iic_start();
	iic_write_byte(_addr + 1);
	while(len)
	{
	 	*buf++ = iic_read_byte();
		if(len == 1)
			iic_send_ack(1);	  //结束，发送非应答，告诉从机不要在发送数据了
		else
			iic_send_ack(0);
		len--;
	}
   iic_stop();
}


