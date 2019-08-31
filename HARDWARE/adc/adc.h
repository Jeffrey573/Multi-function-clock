/******************************ADC头文件************************
* 作者：宁晓兰
******************************************************************/

#ifndef _ADC_H
#define _ADC_H

#include "sys.h"
#include "led.h"
#include "delay.h"

#define PA0 PAin(0)
#define PA1 PAin(1)

#define V_Ro PCin(4) //测光
#define V_Rt PCin(5) //测温


void VoltageAdcInit(void); //端口初始化，ADC1初始化，ADC2初始化
void VoltageAdc1Init(void); //电压测量ADC1初始化
void VoltageAdc2Init(void); //电压测量ADC2初始化
float GetVoltage(u8 adcx, u8 ch); //获取电压值
void VoltageDisplay(u8 adcx, float vol); //显示电压值

u16 GetAdc(u8 adcx, u8 ch); //获取ADC

void UltrasonicAdcInit(void);//超声波测量ADC初始化
u16 GetUltrasonicAdc(u8 ch); 

/******************温度和光照***********************/
void TemperatureAndLightAdcInit(void); //端口，ADC1初始化，通道14、15
void AdcInitForInternalChannels(void);
u16 GetTemperatureAdc(u8 ch);
float GetInternalTemperature( void );
float GetTemperature(void);
u16 GetLightAdc(u8 ch);

#endif

