#ifndef __SYN6288_H
#define __SYN6288_H

#include "typedef.h"
#include "stc12c5a32s2.h"

sbit syn6288_busy = P1^6;

// syn6288 初始化
void syn6288_init();
// 语音合成
void syn6288_speak(uint8_t bgm, cstr text);

#endif 
