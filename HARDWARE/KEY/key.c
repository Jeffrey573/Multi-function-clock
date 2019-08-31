#include "key.h"
#include "delay.h"
//按键输入 驱动代码		   
//PC0.1.2 设置成输入
void KEY_Init(void)
{
	RCC->APB2ENR|=1<<4;     //使能PORTC时钟
	GPIOC->CRL&=0XFFFFF000;//PC0-2设置成输入	  
	GPIOC->CRL|=0X00000888;   
} 
//按键处理函数
//返回按键值
//0，没有任何按键按下
//1，KEY1按下
//2，KEY2按下
//3，KEY3按下
//注意此函数有响应优先级,KEY1>KEY2>KEY3!!
u8 KEY_Scan(void)
{	 
	static u8 key_up=1;//按键按松开标志
	
	if(key_up && (KEY1==0 || KEY2==0 || KEY3==0))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY1==0)
		{
			return 1;
		}
		else if(KEY2==0)
		{
			return 2;
		}
		else if(KEY3==0)
		{
			return 3;
		}
	}
	else if(KEY1==1 && KEY2==1 && KEY3==1)
		key_up=1;
	
	return 0;// 无按键按下
}




















