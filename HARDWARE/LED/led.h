/*********************二极管及数码管头文件********************
* 
******************************************************************/

#ifndef __LED_H
#define __LED_H
	 
#include "sys.h"

//LED端口定义
#define LED_SEL PBout(3) //PB3 二极管灯？数码管

//位选
#define SEL0 PBout(0)
#define SEL1 PBout(1)
#define SEL2 PBout(2)

//段选
#define LED0 PEout(8)
#define LED1 PEout(9)
#define LED2 PEout(10)
#define LED3 PEout(11)
#define LED4 PEout(12)
#define LED5 PEout(13)
#define LED6 PEout(14)
#define LED7 PEout(15)

void LED_Init(void);//初始化
void LedValue( u8 value ); //设置数码管E-A的值；流水灯L7-L0的值	
void SetLed(u8 w, u8 value); //设置数码管哪位显示什么数字
void PortationDisplay(u8 w, u8 value); //数码管显示带小数点的数值
	 				    
#endif


