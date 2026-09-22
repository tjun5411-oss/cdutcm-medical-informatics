#ifndef __KEY_H
#define __KEY_H

#include "typedef.h"
#include "stc12c5a32s2.h"

#define FIRST_CNT 3
#define PERIOD_CNT 1

// 4 个独立按键
sbit key_Switch = P2^3;
sbit key_Pre = P2^2;
sbit key_Nex = P2^1;
sbit key_Enter = P2^0;

// 按键值枚举
typedef enum {
	NoKey = 0x00,
	Switch = 0x01,  // 界面切换
	Pre = 0x02,  // 前一个
	Nex = 0x04,  // 下一个
	Enter = 0x08,  // 确认按键
} Key;

// 按键扫描函数，返回按下按键的按键值
// 支持多个按键同时按下
// 支持短按和长按
Key key_scan();

#endif 
