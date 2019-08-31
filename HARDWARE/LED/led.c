/*********************二极管及数码管初始化或实现********************
* 
******************************************************************/

#include "led.h"

/***************************数码管段选***************************/
u8 segTable[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40};
u8 segTablePortation[] = {0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,0x87,0xff,0xef};

/****************************************************
* 初始化二极管LED灯的引脚端口
******************************************************/
void LED_Init(void)
{
	RCC->APB2ENR|=1<<0;	//使能AFIO
	RCC->APB2ENR|=1<<3;  //使能PORTB时钟
	RCC->APB2ENR|=1<<6;	//使能PORTE时钟	 
	   	 
	AFIO->MAPR |= 0x02000000; //设置PB.3为I/O口可用，且可以SW仿真
	//GPIOB->CRL &= 0xFFFF0FFF;
	//GPIOB->CRL |= 0x00003000; //PB.3推挽输出
	//GPIOB->ODR |= 0x00000008; //PB.3输出高
	GPIOB->CRL &= 0xFFFF0000;
	GPIOB->CRL |= 0x00003333; //PB.3推挽输出
	GPIOB->ODR |= 0x000000FF; //PB.3输出高											  
	
	GPIOE->CRH&=0X00000000;
	GPIOE->CRH|=0X33333333; //PE.8-15推挽输出
	GPIOE->ODR|=0x00000000; //PE.8-15输出低
	
}

/***************************************
* 流水灯选择，或数码管段选
* value:显示的数值对应的段选二进制值
****************************************/
void LedValue(u8 value)
{
	GPIOE->ODR &= ~(0xff<<8);
	GPIOE->ODR |= value<<8;
	 /** LED0 = (value&0x01)?1:0;
		LED1 = (value&0x02)?1:0;
		LED2 = (value&0x04)?1:0;
		LED3 = (value&0x08)?1:0;
		LED4 = (value&0x10)?1:0;
		LED5 = (value&0x20)?1:0;
		LED6 = (value&0x40)?1:0;
		LED7 = (value&0x80)?1:0;*/	
}

/***************************************
* 数码管显示不带小数点的数值
* 参数 w:显示的位置，即位选，左-右：0-7
*      value:要显示的数值
****************************************/
void SetLed(u8 w, u8 value)
{
	SEL0 = w%2;
	SEL1 = w/2%2;
	SEL2 = w/4;
	LedValue(segTable[value]);
}

/***************************************
* 数码管显示带小数点的数值
* 参数 w:显示的位置，即位选，左-右：0-7
*      value:要显示的数值
****************************************/
void PortationDisplay(u8 w, u8 value)
{
	SEL0 = w%2;
	SEL1 = w/2%2;
	SEL2 = w/4;
	LedValue( segTablePortation[value] );
}


