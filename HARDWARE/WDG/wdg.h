#ifndef __WDG_H
#define __WDG_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//看门狗 驱动代码		   
////////////////////////////////////////////////////////////////////////////////// 	  

void IWDG_Init(u8 prer,u16 rlr);	 //初始化IWDG
void IWDG_Feed(void);				 //IWDG喂狗
void WWDG_Init(u8 tr,u8 wr,u8 fprer);//初始化WWDG
void WWDG_Set_Counter(u8 cnt);       //设置WWDG的计数器
#endif




























