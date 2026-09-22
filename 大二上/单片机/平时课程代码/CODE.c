#include<reg51.h>
#include <intrins.h>

#define uint unsigned int
#define uchar unsigned char

sbit adrd = P3^7;    
sbit adwr = P3^6;    
sbit dula = P2^6;
sbit wela = P2^7;   

uchar adval;       

void delay(uint z)
{
    uint x, y;
    for(x = z; x > 0; x--)
        for(y = 110; y > 0; y--);
}

uchar code table[] = {
    0x3f, 0x06, 0x5b, 0x4f,  
    0x66, 0x6d, 0x7d, 0x07,  
    0x7f, 0x6f, 0x77, 0x7c, 
    0x39, 0x5e, 0x79, 0x71   
};

void display(uchar bai_c, uchar sh_c, uchar g_c)
{  
    P0 = table[bai_c];
    dula = 1;
    dula = 0;
    P0 = 0x7e;      
    wela = 1;
    wela = 0;
    delay(2);
    
    P0 = table[sh_c];
    dula = 1;
    dula = 0;
    P0 = 0x7d;     
    wela = 1;
    wela = 0;
    delay(2);
    
    P0 = table[g_c];
    dula = 1;
    dula = 0;
    P0 = 0x7b;      
    wela = 1;
    wela = 0;
    delay(2);
}

uchar read_adc(void)
{
    uchar temp;
    
    adwr = 1;
    _nop_();
    adwr = 0;      
    _nop_();
    adwr = 1;      
    
    delay(1);      
    
    P1 = 0xff;     
    adrd = 1;
    _nop_();
    adrd = 0;      
    _nop_();
    temp = P1;   
    adrd = 1;      
    
    return temp;
}

void main()
{
    uchar A1, A2, A3;
    
    wela = 1;
    P0 = 0x7f;      
    wela = 0;
    
    dula = 1;
    P0 = 0x00;
    dula = 0;
    
    while(1)
    {
        adval = read_adc();
        
        A1 = adval / 100;        
        A2 = adval % 100 / 10;   
        A3 = adval % 10;          
        display(A1, A2, A3);
        
        
    }
}