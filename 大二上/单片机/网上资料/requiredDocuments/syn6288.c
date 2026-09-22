#include "syn6288.h"
#include "uart.h"
#include "delay.h"
#include <string.h>

//static code uint8_t SYN_StopCom[] = { 0xFD, 0x00, 0x02, 0x02, 0xFD, 0x00}; //停止合成
//static code uint8_t SYN_SuspendCom[] = { 0xFD, 0x00, 0x02, 0x03, 0xFC, 0x00}; //暂停合成
//static code uint8_t SYN_RecoverCom[] = { 0xFD, 0x00, 0X02, 0x04, 0xFB, 0x00}; //恢复合成
//static code uint8_t SYN_ChackCom[] = { 0xFD, 0x00, 0x02, 0x21, 0xDE, 0x00}; //状态查询

void syn6288_init() {
    uart2_init();
}

//bgm:  0:无背景音乐  1~15:选择背景音乐
void syn6288_speak(uint8_t bgm, cstr text) {
    static pdata uint8_t Frame_Info[6];
    static pdata uint8_t n, ecc;
    data uint8_t i;
    /****************需要发送的文本**********************************/
    ecc = 0;
    n = strlen(text); 			//需要发送文本的长度
    /*****************帧固定配置信息**************************************/
    Frame_Info[0] = 0xFD ; 			//构造帧头FD
    uart2_sendByte(0xFD);
    Frame_Info[1] = 0x00 ; 			//构造数据区长度的高字节
    uart2_sendByte(0x00);
    Frame_Info[2] = n + 3; 		//构造数据区长度的低字节
    uart2_sendByte(n + 3);
    Frame_Info[3] = 0x01 ; 			//构造命令字：合成播放命令
    uart2_sendByte(0x01);
    Frame_Info[4] = 0x00 | bgm << 3 ; //构造命令参数：背景音乐设定
    uart2_sendByte(0x00 | bgm << 3);
    for (i = 0; i < n; ++i) {
        uart2_sendByte(text[i]);
    }
    /*******************校验码计算***************************************/
    //依次发送构造好的5个帧头字节
    for(i = 0; i < 5; i++) {
        ecc = ecc ^ (Frame_Info[i]);		//对发送的字节进行异或校验
    }
    //依次发送待合成的文本数据
    for(i = 0; i < n; i++) {
        ecc = ecc ^ (text[i]); 				//对发送的字节进行异或校验
    }
    uart2_sendByte(ecc);
}
