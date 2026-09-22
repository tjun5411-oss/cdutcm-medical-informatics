#include <reg52.h>
#include <string.h>
#include <intrins.h>
#include <math.h>

#define uchar unsigned char
#define uint unsigned int
#define TRUE 1
#define FALSE 0

sbit dula = P2^6;
sbit wela = P2^7;
sbit beep = P2^3;
sbit alarm_led = P1^0;
sbit status_led = P1^1;
sbit K1 = P3^4;
sbit SCL = P1^5;
sbit SDA = P1^4;

uchar code num_table[] = {
    0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f,
    0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71,
    0x38, 0x73, 0x3e, 0x76, 0x00
};
uchar display_buf[4] = {0, 0, 0, 0};

#define CHAR_F 15
#define CHAR_A 10
#define CHAR_L 16
#define CHAR_P 17
#define CHAR_E 14
#define CHAR_H 19
#define CHAR_BLANK 20

#define SMPLRT_DIV      0x19
#define CONFIG          0x1A
#define GYRO_CONFIG     0x1B
#define ACCEL_CONFIG    0x1C
#define ACCEL_XOUT_H    0x3B
#define ACCEL_YOUT_H    0x3D
#define ACCEL_ZOUT_H    0x3F
#define PWR_MGMT_1      0x6B
#define WHO_AM_I        0x75
#define SlaveAddress    0xD0

#define STATE_NORMAL 0
#define STATE_FALL_DETECTED 1
#define STATE_ALARMING 2

short accel_x, accel_y, accel_z;
uchar system_state = STATE_NORMAL;
uint fall_timer = 0;
bit alarm_request = FALSE;
bit reset_request = FALSE;

void delay_1ms(unsigned char x)
{
    unsigned char y, z;
    for(z = x; z > 0; z--)
        for(y = 114; y > 0; y--);
}

void delay_ms(uint ms)
{
    uint i, j;
    for(i = ms; i > 0; i--)
        for(j = 110; j > 0; j--);
}

void iic_delay(void)
{
    _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
}

void iic_start(void)
{
    SDA = 1;
    iic_delay();
    SCL = 1;
    iic_delay();
    SDA = 0;
    iic_delay();
    SCL = 0;
}

void iic_stop(void)
{
    SDA = 0;
    iic_delay();
    SCL = 0;
    iic_delay();
    SCL = 1;
    iic_delay();
    SDA = 1;
    iic_delay();
}

void iic_send_ack(bit _ack)
{
    SDA = _ack;
    SCL = 1;
    iic_delay();
    SCL = 0;
    iic_delay();
}

void iic_wait_ack(void)
{
    unsigned char i = 0;
    SCL = 1;
    iic_delay();
    while((SDA == 1) && (i < 250)) i++;
    SCL = 0;
}

void iic_write_byte(uchar _byte)
{
    uchar i;
    for(i = 0; i < 8; i++)
    {
        SDA = _byte & 0x80;
        iic_delay();
        SCL = 1;
        iic_delay();
        SCL = 0;
        iic_delay();
        _byte = _byte << 1;
    }
    SDA = 1;
    iic_wait_ack();
}

uchar iic_read_byte(void)
{
    uchar i = 0;
    uchar read_byte = 0;
    SDA = 1;
    iic_delay();
    for(i = 0; i < 8; i++)
    {
        SCL = 1;
        read_byte = (read_byte << 1) | SDA;
        iic_delay();
        SCL = 0;
        iic_delay();
    }
    return read_byte;
}

void iic_write_addr_byte(uchar _addr, uchar _byte)
{
    iic_start();
    iic_write_byte(SlaveAddress);
    iic_write_byte(_addr);
    iic_write_byte(_byte);
    iic_stop();
}

uchar iic_read_addr_byte(uchar _addr)
{
    uchar read_data;
    iic_start();
    iic_write_byte(SlaveAddress);
    iic_write_byte(_addr);
    iic_start();
    iic_write_byte(SlaveAddress + 1);
    read_data = iic_read_byte();
    iic_send_ack(1);
    iic_stop();
    return read_data;
}

void iic_continue_read(uchar _addr, uchar _reg, uchar len, uchar *buf)
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
            iic_send_ack(1);
        else
            iic_send_ack(0);
        len--;
    }
    iic_stop();
}

void iic_init(void)
{
    SCL = 1;
    iic_delay();
    SDA = 1;
    iic_delay();
}

uchar mpu6050_init(void)
{
    if(iic_read_addr_byte(WHO_AM_I) != 0x68)
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

void mpu6050_read_all(void)
{
    uchar buff[6];
    
    iic_continue_read(SlaveAddress, ACCEL_XOUT_H, 6, buff);
    accel_x = (short)((buff[0] << 8) | buff[1]);
    accel_y = (short)((buff[2] << 8) | buff[3]);
    accel_z = (short)((buff[4] << 8) | buff[5]);
}

bit CheckFall(void)
{
    long acc_total_sq = (long)accel_x * accel_x + 
                       (long)accel_y * accel_y + 
                       (long)accel_z * accel_z;
    
    float ax = (float)accel_x / 4096.0;
    float ay = (float)accel_y / 4096.0;
    float az = (float)accel_z / 4096.0;
    
    float pitch = atan2(ax, sqrt(ay*ay + az*az)) * 180.0 / 3.14159;
    float roll = atan2(ay, sqrt(ax*ax + az*az)) * 180.0 / 3.14159;
    
    if((acc_total_sq < 500000000L || acc_total_sq > 1200000000L) &&
       (abs(pitch) > 45 || abs(roll) > 45))
    {
        return TRUE;
    }
    
    return FALSE;
}

void SetDisplay(uchar d1, uchar d2, uchar d3, uchar d4)
{
    display_buf[0] = d1;
    display_buf[1] = d2;
    display_buf[2] = d3;
    display_buf[3] = d4;
}

void DisplayNormal(void)
{
    SetDisplay(CHAR_BLANK, CHAR_BLANK, CHAR_BLANK, CHAR_BLANK);
}

void DisplayFall(void)
{
    SetDisplay(CHAR_F, CHAR_A, CHAR_L, CHAR_L);
}

void DisplayHelp(void)
{
    SetDisplay(CHAR_H, CHAR_E, CHAR_L, CHAR_P);
}

void display_scan(void)
{
    static uchar pos = 0;
    uchar code bit_table[] = {0xfe, 0xfd, 0xfb, 0xf7};
    
    dula = 0;
    wela = 0;
    P0 = 0x00;
    
    dula = 1;
    P0 = num_table[display_buf[pos]];
    dula = 0;
    
    wela = 1;
    P0 = bit_table[pos];
    wela = 0;
    
    if(++pos >= 4) pos = 0;
}

uchar key_scan(void)
{
    if(K1 == 0)
    {
        delay_ms(20);
        if(K1 == 0)
        {
            while(K1 == 0);
            return 1;
        }
    }
    return 0;
}

void UART1_Init(void)
{
    TMOD |= 0x20;
    TH1 = 0xFD;
    TL1 = 0xFD;
    TR1 = 1;
    SCON = 0x50;
    EA = 1;
}

void UART1_SendByte(uchar DAT)
{
    SBUF = DAT;
    while(!TI);
    TI = 0;
}

void SYN_PlayText(uchar *text, uchar len)
{
    uchar Frame_Info[64];
    uchar ecc = 0;
    uchar i;
    
    Frame_Info[0] = 0xFD;
    Frame_Info[1] = (len + 3) >> 8;
    Frame_Info[2] = (len + 3) & 0xFF;
    Frame_Info[3] = 0x01;
    Frame_Info[4] = 0x01;
    
    for(i = 0; i < 5; i++)
    {
        ecc ^= Frame_Info[i];
    }
    for(i = 0; i < len; i++)
    {
        ecc ^= text[i];
    }
    
    memcpy(Frame_Info+5, text, len);
    Frame_Info[5+len] = ecc;
    
    for(i = 0; i < 6+len; i++)
    {
        UART1_SendByte(Frame_Info[i]);
    }
}

void ResetSystemState(void)
{
    system_state = STATE_NORMAL;
    fall_timer = 0;
    alarm_request = FALSE;
    DisplayNormal();
    alarm_led = 1;
    beep = 1;
}

void Timer0_Isr(void) interrupt 1
{
    static uint mpu_read_cnt = 0;
    
    TH0 = (65536 - 1000) / 256;
    TL0 = (65536 - 1000) % 256;
    
    display_scan();
    
    mpu_read_cnt++;
    if(mpu_read_cnt >= 50)
    {
        mpu_read_cnt = 0;
        mpu6050_read_all();
    }
    
    if(key_scan() == 1)
    {
        reset_request = TRUE;
        return;
    }
    
    switch(system_state)
    {
        case STATE_NORMAL:
            status_led = ~status_led;
            if(CheckFall())
            {
                system_state = STATE_FALL_DETECTED;
                fall_timer = 0;
                DisplayFall();
            }
            break;
            
        case STATE_FALL_DETECTED:
            alarm_led = 0;
            beep = 0;
            fall_timer++;
            if(fall_timer >= 3000)
            {
                system_state = STATE_ALARMING;
                DisplayHelp();
                alarm_request = TRUE;
            }
            break;
            
        case STATE_ALARMING:
            if(fall_timer % 500 < 250)
            {
                alarm_led = 0;
                beep = 0;
            }
            else
            {
                alarm_led = 1;
                beep = 1;
            }
            fall_timer++;
            break;
    }
}

void main(void)
{
    uchar alarm_text[] = {
    0xCE, 0xD2,  // ?
    0xBA, 0xC3,  // ?
    0xCF, 0xF1,  // ?
    0xD3, 0xD0,  // ?
    0xB5, 0xE3,  // ?
    0xCB, 0xC0,  // ?
    0xC1, 0xCB,
    0xBE, 0xC8,  // ?
    0xBE, 0xC8,  // ?
    0xCE, 0xD2	// ?
};
    
    UART1_Init();
    iic_init();
    delay_ms(100);
    
    if(!mpu6050_init())
    {
        while(1)
        {
            alarm_led = 0;
            delay_ms(200);
            alarm_led = 1;
            delay_ms(200);
        }
    }
    
    TMOD |= 0x01;
    TH0 = (65536 - 1000) / 256;
    TL0 = (65536 - 1000) % 256;
    ET0 = 1;
    TR0 = 1;
    EA = 1;
    
    ResetSystemState();
    
    while(1)
    {
        delay_ms(100);
        
        if(reset_request)
        {
            reset_request = FALSE;
            ResetSystemState();
        }
        
        if(alarm_request)
        {
            alarm_request = FALSE;
            SYN_PlayText(alarm_text, 20);
        }
    }
}