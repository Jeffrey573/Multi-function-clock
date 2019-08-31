#ifndef _RTC_H
#define _RTC_H

#include "sys.h"

//时间结构体
typedef struct
{
	u8 hour;
	u8 minute;
	u8 sec;
	//公历
	u16 year;
	u8 month;
	u8 date;
	u8 week;
}tm;

extern tm timer;
extern u8 const mon_table[12]; //月份日期数据表
void DispTimer(u8 x, u8 y, u8 size); //在指定位置开始显示时间
void DispWeek(u8 x, u8 y, u8 size, u8 lang); //在指定位置显示星期
u8 RtcInit(void); //初始化RTC，成功返回0， 失败返回1
u8 IsLeapYear(u16 y); //平年，闰年判断
u8 RtcGet(void); //更新时间
u8 RtcGetWeek(u16 y, u8 m, u8 day);
u8 RtcSet(u16 sy, u8 sm, u8 sd, u8 hour, u8 min, u8 sec); //设置时间
void AutoTimeSet(void); //设置时间为编译时间



#endif



