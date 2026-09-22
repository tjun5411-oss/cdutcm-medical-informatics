
#include "delay.h"
#include <intrins.h>

/*10us*/
 void delay_10us(unsigned char x)//12MHZ
{
	unsigned char y;
	for(y=x;y>0;y--){

	   _nop_();
	   _nop_();
	   _nop_();
	   _nop_();
	   _nop_();
	   _nop_();	
	}
} 
/*1ms*/
 void delay_1ms(unsigned char x) //ÑÓÊ±XºÁÃë
{
	unsigned char y,z;

	for(z=x;z>0;z--)
		for(y=114;y>0;y--);
}
/*500ms*/
void delay_500ms(unsigned char x)
{	
	unsigned char i,j,k,l;

	for(l=x;l>0;l--)
		for(i=15;i>0;i--)
		     for(j=202;j>0;j--)
		         for(k=81;k>0;k--);
}


