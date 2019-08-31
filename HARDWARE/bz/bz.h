/*********************蜂鸣器头文件*****************************
* 
***************************************************************/

#ifndef _BZ_H
#define _BZ_H

#include "sys.h"
#include "delay.h"
//蜂鸣器引脚
#define beep PEout(5)

void BzInit(void);
u8 QuYin(u8 qu);  //找到对应音符的应设置的定时翻转时间
void PlayMusic(u16 song); //播放对应的音节字符
#endif



