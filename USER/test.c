#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h" 
#include "key.h"
#include "exti.h"
#include "timer.h"
#include "rtc.h"
#include "adc.h"
#include "bz.h"

const u8 *COMPILED_DATE=__DATE__;//获得编译日期
const u8 *COMPILED_TIME=__TIME__;//获得编译时间	 	 
#define uchar unsigned char
extern int rhour;
extern int rminute;
extern int rsecond;
int flag=0;
extern int gKey_hour;
extern int gKey_minute;
extern int gKey_sec;
int ehour=0;
int eminute=0;
int esecond=0;  
int main(void)
{	
	u8 key=0,i=0;		
 	Stm32_Clock_Init(6); //系统时钟设置
	delay_init(72);	     //延时初始化
	uart_init(72,9600);  //串口初始化 
	LED_Init();		  	 //初始化与LED连接的硬件接口
	LED_SEL = 0;
	for(i=0;i<8;i++)
	{
		SetLed(i,10);
		delay_ms(180);
	}
	KEY_Init();
	RtcInit(); 
   	while(1)
	{
		key = KEY_Scan();
		SetLed(0,timer.hour/10);delay_ms(1);
		SetLed(1,timer.hour%10);delay_ms(1);
		SetLed(2,10);delay_ms(1);
		SetLed(3,timer.minute/10);delay_ms(1);
		SetLed(4,timer.minute%10);delay_ms(1);
		SetLed(5,10);delay_ms(1);
		SetLed(6,timer.sec/10);delay_ms(1);
		SetLed(7,timer.sec%10);delay_ms(1);

		if( key==3)
		{
			while(1){
				key = KEY_Scan();
				SetLed(0,rhour/10);delay_ms(1);
				SetLed(1,rhour%10);delay_ms(1);
				SetLed(2,10);delay_ms(1);
				SetLed(3,rminute/10);delay_ms(1);
				SetLed(4,rminute%10);delay_ms(1);
				SetLed(5,10);delay_ms(1);
				SetLed(6,rsecond/10);delay_ms(1);
				SetLed(7,rsecond%10);delay_ms(1);
				if(key==3){
					//u8 t,len;
					u8 k=0;
					flag=1;
					Timerx_Init(10000,7199);//10Khz的计数频率，计数到5000为500ms
					while(1){
						key = KEY_Scan();
						SetLed(0,rhour/10);delay_ms(1);
						SetLed(1,rhour%10);delay_ms(1);
						SetLed(2,10);delay_ms(1);
						SetLed(3,rminute/10);delay_ms(1);
						SetLed(4,rminute%10);delay_ms(1);
						SetLed(5,10);delay_ms(1);
						SetLed(6,rsecond/10);delay_ms(1);
						SetLed(7,rsecond%10);delay_ms(1);
						if(key==1)
						{
							printf("捕获到第%d个时间点%d:%d:%d\n",++k,rhour,rminute,rsecond);
						}	
					}	
				}	
			}					
		}
		
		else if(key==2)
		{
			EXTIX_Init();
			while(1)
			{
				key = KEY_Scan();
				SetLed(0,gKey_hour/10);delay_ms(1);
				SetLed(1,gKey_hour%10);delay_ms(1);
				SetLed(2,10);delay_ms(1);
				SetLed(3,gKey_minute/10);delay_ms(1);
				SetLed(4,gKey_minute%10);delay_ms(1);
				SetLed(5,10);delay_ms(1);
				SetLed(6,gKey_sec/10);delay_ms(1);
				SetLed(7,gKey_sec%10);delay_ms(1);
				if(key==1)
				{
					Timerx_Init(10000,7199);//10Khz的计数频率，计数到5000为500ms
					while(1)
					{
						key = KEY_Scan();
						SetLed(0,gKey_hour/10);delay_ms(1);
						SetLed(1,gKey_hour%10);delay_ms(1);
						SetLed(2,10);delay_ms(1);
						SetLed(3,gKey_minute/10);delay_ms(1);
						SetLed(4,gKey_minute%10);delay_ms(1);
						SetLed(5,10);delay_ms(1);
						SetLed(6,gKey_sec/10);delay_ms(1);
						SetLed(7,gKey_sec%10);delay_ms(1);
					}	
				}	
			}		
		}
		
		else if(key==1)
		{
			ehour=timer.hour;
			eminute=timer.minute;
			esecond=timer.sec;
			EXTIX_Init();
			while(1)
			{
				key = KEY_Scan();
				SetLed(0,gKey_hour/10);delay_ms(1);
				SetLed(1,gKey_hour%10);delay_ms(1);
				SetLed(2,10);delay_ms(1);
				SetLed(3,gKey_minute/10);delay_ms(1);
				SetLed(4,gKey_minute%10);delay_ms(1);
				SetLed(5,10);delay_ms(1);
				SetLed(6,gKey_sec/10);delay_ms(1);
				SetLed(7,gKey_sec%10);delay_ms(1);
				if(key==1)
				{
					if((gKey_sec-esecond)<0)
					{
						gKey_minute--;
						gKey_sec = gKey_sec+60 - esecond;	
					}else{
						gKey_sec -=esecond;
					}
					if((gKey_minute-eminute)<0)
					{
						gKey_hour--;
						gKey_minute = gKey_minute+60 - eminute;	
					}else{
						gKey_minute -=eminute;	
					}
					if((gKey_hour-ehour)<0)
					{
						break;
					}else{
						gKey_hour -=ehour;	
					}
					Timerx_Init(10000,7199);//10Khz的计数频率，计数到5000为500ms
					while(1)
					{
						key = KEY_Scan();
						SetLed(0,timer.hour/10);delay_ms(1);
						SetLed(1,timer.hour%10);delay_ms(1);
						SetLed(2,10);delay_ms(1);
						SetLed(3,timer.minute/10);delay_ms(1);
						SetLed(4,timer.minute%10);delay_ms(1);
						SetLed(5,10);delay_ms(1);
						SetLed(6,timer.sec/10);delay_ms(1);
						SetLed(7,timer.sec%10);delay_ms(1);
					}	
				}	
			}	
		}  
	}	 
}

























