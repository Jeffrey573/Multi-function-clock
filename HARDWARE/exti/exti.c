#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "timer.h"
//////////////////////////////////////////////////////////////////////////////////	 
//外部中断 驱动代码			   
////////////////////////////////////////////////////////////////////////////////// 	
int time[3]={0,0,0};
int sub=3;  
int gKey_hour = 0;
int gKey_minute = 0;
int gKey_sec = 0;
//外部中断0服务程序
void EXTI0_IRQHandler(void)
{
	delay_ms(10);//消抖
	if(KEY3==0)	 //按键3
	{
		sub--;
		if(sub<0)
		{
			sub=2;
		}
	}		 
	EXTI->PR=1<<0;  //清除LINE0上的中断标志位  
}

//外部中断1服务程序
void EXTI1_IRQHandler(void)
{
	delay_ms(10);//消抖
	if(KEY2==0)	 //按键2
	{
		if(sub==2)
		{
			gKey_sec++;
			if(gKey_sec>59)
			{
				gKey_sec=0;
			}
		}else if(sub==1){
			gKey_minute++;
			if(gKey_minute>59)
			{
				gKey_minute=0;
			}	
		}else if(sub==0){
			gKey_hour++;
			if(gKey_hour>23)
			{
				gKey_hour=0;
			}
		}	
	}		 
	EXTI->PR=1<<1;  //清除LINE1上的中断标志位  
}

//外部中断2服务程序
void EXTI2_IRQHandler(void)
{
	delay_ms(10);//消抖
	if(KEY1==0)	 //按键1
	{
		//	
	}		 
	EXTI->PR=1<<2;  //清除LINE2上的中断标志位  
}

//外部中断初始化程序
//初始化PC0-2为中断输入.
void EXTIX_Init(void)
{
	RCC->APB2ENR|=1<<4;     //使能PORTC时钟
	GPIOC->CRL&=0XFFFFF000;//PC0-2设置成输入	  
	GPIOC->CRL|=0X00000888;
	
	Ex_NVIC_Config(GPIO_C,0,FTIR);//下降沿触发
	Ex_NVIC_Config(GPIO_C,1,FTIR);//下降沿触发
	Ex_NVIC_Config(GPIO_C,2,FTIR);//下降沿触发

	MY_NVIC_Init(2,2,EXTI0_IRQChannel,2);//抢占2，子优先级2，组2
	MY_NVIC_Init(2,1,EXTI1_IRQChannel,2);//抢占2，子优先级1，组2	   
	MY_NVIC_Init(2,0,EXTI2_IRQChannel,2);//抢占2，子优先级1，组2
}












