#include "timer.h"
#include "led.h"
#include "bz.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//ͨ�ö�ʱ�� ��������			   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/12/03
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 
#define uchar unsigned char	  
int rhour;
int rminute;
int rsecond;
extern int flag;
extern int gKey_hour;
extern int gKey_minute;
extern int gKey_sec;
//��ʱ��3�жϷ������	 
void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR & 0x0001)//����ж�
	{
		//LED5=!LED5;
		delay_ms(10);//����
		if(flag)
		{
			rsecond++;
			if(rsecond>59){
				rsecond=0;
				rminute++;
				if(rminute>59){
					rminute=0;
					rhour++;
					if(rhour>23){
						rhour=0;
					}
				}
			}	
		}else{
			gKey_sec--;
			if(gKey_sec<0)
			{
				gKey_sec=59;
				gKey_minute--;
				if(gKey_minute<0)
				{
					gKey_minute=59;
					gKey_hour--;
					if(gKey_hour<0){
						LED_SEL =1;
						BzInit();
						while(1){
				  			LED5=!LED5;
							delay_ms(10);
				  			beep=~beep;
						}
					}		
				}
			}	
		}			    				   				     	    	
	}				   
	TIM3->SR &= ~(1<<0);//����жϱ�־λ 	    
}
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void Timerx_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR |= 1<<1;//TIM3ʱ��ʹ��  
 	TIM3->ARR = arr;  //�趨�������Զ���װֵ//�պ�1ms    
	TIM3->PSC = psc;  //Ԥ��Ƶ��7200,�õ�10Khz�ļ���ʱ��
	//������Ҫͬʱ���òſ���ʹ���ж�
	TIM3->DIER |= 1<<0;   //��������ж�				
	TIM3->DIER |= 1<<6;   //�������ж�	   
	TIM3->CR1 |= 0x01;    //ʹ�ܶ�ʱ��3

  	MY_NVIC_Init(1,3,TIM3_IRQChannel,2);//��ռ1�������ȼ�3����2									 
}














