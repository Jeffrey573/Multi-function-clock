#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "timer.h"
//////////////////////////////////////////////////////////////////////////////////	 
//�ⲿ�ж� ��������			   
////////////////////////////////////////////////////////////////////////////////// 	
int time[3]={0,0,0};
int sub=3;  
int gKey_hour = 0;
int gKey_minute = 0;
int gKey_sec = 0;
//�ⲿ�ж�0�������
void EXTI0_IRQHandler(void)
{
	delay_ms(10);//����
	if(KEY3==0)	 //����3
	{
		sub--;
		if(sub<0)
		{
			sub=2;
		}
	}		 
	EXTI->PR=1<<0;  //���LINE0�ϵ��жϱ�־λ  
}

//�ⲿ�ж�1�������
void EXTI1_IRQHandler(void)
{
	delay_ms(10);//����
	if(KEY2==0)	 //����2
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
	EXTI->PR=1<<1;  //���LINE1�ϵ��жϱ�־λ  
}

//�ⲿ�ж�2�������
void EXTI2_IRQHandler(void)
{
	delay_ms(10);//����
	if(KEY1==0)	 //����1
	{
		//	
	}		 
	EXTI->PR=1<<2;  //���LINE2�ϵ��жϱ�־λ  
}

//�ⲿ�жϳ�ʼ������
//��ʼ��PC0-2Ϊ�ж�����.
void EXTIX_Init(void)
{
	RCC->APB2ENR|=1<<4;     //ʹ��PORTCʱ��
	GPIOC->CRL&=0XFFFFF000;//PC0-2���ó�����	  
	GPIOC->CRL|=0X00000888;
	
	Ex_NVIC_Config(GPIO_C,0,FTIR);//�½��ش���
	Ex_NVIC_Config(GPIO_C,1,FTIR);//�½��ش���
	Ex_NVIC_Config(GPIO_C,2,FTIR);//�½��ش���

	MY_NVIC_Init(2,2,EXTI0_IRQChannel,2);//��ռ2�������ȼ�2����2
	MY_NVIC_Init(2,1,EXTI1_IRQChannel,2);//��ռ2�������ȼ�1����2	   
	MY_NVIC_Init(2,0,EXTI2_IRQChannel,2);//��ռ2�������ȼ�1����2
}












