#include "rtc.h"
#include "delay.h"
tm timer; //实时时钟结构体

/****************************************************
* 实时时钟配置
* 初始化RTC时钟,同时检测时钟是否工作正常
* BKP->DR1用于保存是否第一次配置的设置
* 返回0:正常
* 其他:错误代码
******************************************************/
u8 RtcInit(void)
{
	//检查是否是第一次配置时钟
	u8 temp = 0;
	if( BKP->DR1 != 0x5050 )
	{
		RCC->APB1ENR |= 1<<28; //使能电源时钟
		RCC->APB1ENR |= 1<<27; //使能备份时钟
		PWR->CR |= 1<<8; //取消备份去写保护
		RCC->BDCR |= 1<<16; //备份区域软复位
		RCC->BDCR &= ~(1<<16); //备份区域软复位结束
		RCC->BDCR |= 1<<0; //开启外部低速振荡器
		while( ( !(RCC->BDCR&0x02) ) && (temp<250) ) //等待外部时钟就绪
		{
			temp++;
			delay_ms(10);
		}
		if( temp>=250 )
			return 1; //初始化时钟失败，晶振有问题
		RCC->BDCR |= 1<<8; //LSI作为RTC时钟
		RCC->BDCR |= 1<<15; //RTC时钟使能
		while( !(RTC->CRL & (1<<5)) )
			; //等待RTC寄存器操作完成
		while( !(RTC->CRL&(1<<3)) )
			; //等待RTC寄存器同步
		RTC->CRH |= 0x01; //允许秒中断
		while( !(RTC->CRL&(1<<5)) )
			; //等待RTC寄存器操作完成
		RTC->CRL |= 1<<4; //允许配置
		RTC->PRLH = 0x0000;
		RTC->PRLL = 0x7fff; //32767 时钟周期设置(有待观察,看是否跑慢了?)理论值：32767
		AutoTimeSet(); 
		//RtcSet(2014,12,2,10,0,55); //设置时间
		RTC->CRL &= ~(1<<4); //配置更新
		while( !(RTC->CRL & (1<<5)) )
			; //等待RTC寄存器操作完成
		BKP->DR1 = 0x5050;
		//BKP_Write(1,0X5050);;//在寄存器1标记已经开启了 
		//printf("FIRST TIME\n");
	}
	else //系统继续计时
	{
		while( !(RTC->CRL & (1<<3)) )
			; //等待RTC寄存器同步
		RTC->CRH |= 0x01; //允许秒中断
		while( !(RTC->CRL&(1<<5)) )
			; //等待RTC寄存器操作完成
	}
	MY_NVIC_Init(0, 0, RTC_IRQChannel, 2); //RTC,G2,P2,S2.优先级最低  
	RtcGet(); //更新时间
	return 0; //成功
}

/****************************************************
* RTC中断服务函数		
* RTC时钟中断
* 每秒触发一次
******************************************************/
void RTC_IRQHandler(void)
{
	if( RTC->CRL & 0x0001 ) //秒钟中断
	{
		RtcGet(); //更新时间
	}
	if( RTC->CRL & 0x0002 ) //闹钟中断
	{
		RTC->CRL &= ~(0x0002); //清闹钟中断
		//闹钟处理  doing
	}
	RTC->CRL &= 0x0ffa; //清除溢出，秒钟中断标志
	while( !(RTC->CRL&(1<<5)) )
			; //等待RTC寄存器操作完成
}
	
/****************************************************
* 判断是否是闰年函数
* 月份   1  2  3  4  5  6  7  8  9  10 11 12
* 闰年   31 29 31 30 31 30 31 31 30 31 30 31
* 非闰年 31 28 31 30 31 30 31 31 30 31 30 31
* 输入:年份
* 输出:该年份是不是闰年.1,是.0,不是
******************************************************/
u8 IsLeapYear(u16 y)
{
	if( y%4==0 ) //必须能被4整除
	{
		if( y%100==0 )
		{
			if( y%400==0 )
				return 1; //如果以00结尾，且能被400整除
			else
				return 0;
		}
		return 1;
	}
	else
		return 0;
}

/****************************************************
* 设置时钟
* 把输入的时钟转换为秒钟
* 以1970年1月1日为基准
* 1970~2099年为合法年份
* 返回值:0,成功;其他:错误代码.
* 月份数据表
******************************************************/
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表	  
//平年的月份日期表
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};
u8 RtcSet(u16 sy, u8 sm, u8 sd, u8 hour, u8 min, u8 sec)
{
	u16 t;
	u32 seccount = 0;
	if( sy<1970 || sy>2099 )
		return 1;
	for( t=1970; t<sy; t++ ) //把所有年份的秒钟相加
	{
		if( IsLeapYear(t) )
			seccount += 31622400; //闰年的秒钟数
		else //平年的秒钟数
			seccount += 31536000; 
	}
	sm -= 1;
	for( t=0; t<sm; t++ ) //把前面月份的秒钟数相加
	{
		seccount += (u32)mon_table[t] * 86400; //月份秒钟数相加
		if( IsLeapYear(sy)&&t==1 )
			seccount += 86400; //闰年2月份增加一天的秒数
	}
	seccount += (u32)(sd - 1)*86400; //把前面日期的秒钟数相加
	seccount += (u32)hour*3600; //小时秒钟数
	seccount += (u32)min*60;
	seccount += sec; //最后把秒钟加上去
	
	//设置时钟
	RCC->APB1ENR |= 1<<28; //使能电源时钟
	RCC->APB1ENR |= 1<<27; //使能备份时钟
	PWR->CR |= 1<<8; //取消备份区写保护
	//以上三步必须
	RTC->CRL |= 1<<4; //允许配置
	RTC->CNTL = seccount&0xffff;
	RTC->CNTH = seccount>>16;
	RTC->CRL &= ~(1<<4); //配置更新
	while( !(RTC->CRL&(1<<5)) )
		; //等待RTC寄存器操作完成
	return 0;
}

/****************************************************
* 得到当前的时间
* 返回值:0,成功;其他:错误代码.
******************************************************/
u8 RtcGet(void)
{
	static u16 daycnt = 0;
	u32 timecount = 0;
	u32 temp = 0;
	u16 temp1 = 0;
	timecount = RTC->CNTH; //得到计数器中的值（秒钟数）
	timecount <<= 16;
	timecount += RTC->CNTL;
	
	temp = timecount/86400; //得到天数
	if( daycnt!=temp ) //超过了一天
	{
		daycnt = temp;
		temp1 = 1970; //从1970年开始
		while( temp>=365 )
		{
			if( IsLeapYear(temp1) ) //是闰年
			{
				if( temp>=366 )
					temp-=366; //闰年的秒钟数
				else
					break;
			}
			else
				temp-=365; //平年
			temp1++;
		}
		timer.year = temp1; //得到年份
		temp1 = 0;
		while( temp>=28 ) //超过了一个月
		{
			if( IsLeapYear(timer.year) && temp1==1 ) //当年闰年二月份
			{
				if( temp>29 )
					temp-=29; //闰年的秒钟数
				else
					break;
			}
			else
			{
				if( temp>=mon_table[temp1] )
					temp-=mon_table[temp1]; //平年
				else 
					break;
			}
			temp1++;
		}
		timer.month = temp1+1; //得到月份
		timer.date = temp+1; //得到日期
	}
	temp = timecount%86400; //得到秒钟数
	timer.hour = temp/3600; //小时
	timer.minute = (temp%3600)/60; //分钟
	timer.sec = (temp%3600)%60; //秒钟
	timer.week = RtcGetWeek( timer.year, timer.month, timer.date); //获取星期
	return 0;
}

/****************************************************
* 获得现在是星期几
* 功能描述:输入公历日期得到星期(只允许1901-2099年)
* 输入参数：公历年月日 
* 返回值：星期号
******************************************************/
u8 RtcGetWeek(u16 y, u8 m, u8 day)
{
	u16 temp;
	u8 yearH, yearL;
	
	yearH = y/100;
	yearL = y%100;
	//如果为21世纪，年份数加100
	if( yearH>19 )
		yearL += 100;
	//所过闰年数只算1900年之后的
	temp = yearL + yearL/4;
	temp = temp%7;
	temp = temp+day+table_week[m-1];
	if( yearL%4==0 && m<3 )
		temp--;
	return temp%7;
}

/****************************************************
* 比较两个字符串指定长度的内容是否相等
* 参数:s1,s2要比较的两个字符串;len,比较长度
* 返回值:1,相等;0,不相等
******************************************************/
u8 str_cmpx(u8 *s1, u8 *s2, u8 len)
{
	u8 i;
	for( i=0; i<len; i++ )
		if( (*s1++) != *s2++ )
			return 0;
	return 1;
}

extern const u8 *COMPILED_DATE; //获得编译日期
extern const u8 *COMPILED_TIME; //获得编译时间

const u8 MonthTab[12][3] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"}; 
/****************************************************
* 自动设置时间为编译器时间
******************************************************/
void AutoTimeSet(void)
{
	u8 temp[3];
	u8 i;
	u8 mon, date;
	u16 year;
	u8 sec, min, hour;
	for( i=0; i<3; i++ )
		temp[i] = COMPILED_DATE[i]; 
	for( i=0; i<12; i++ )
		if( str_cmpx( (u8*)MonthTab[i], temp, 3 ) )
			break;
	mon = i + 1; //得到月份
	if( COMPILED_DATE[4]==' ' )
		date = COMPILED_DATE[5] - '0';
	else
		date = 10*(COMPILED_DATE[4]-'0') + COMPILED_DATE[5]-'0';
	year = 1000*(COMPILED_DATE[7]-'0')+100*(COMPILED_DATE[8]-'0')+10*(COMPILED_DATE[9]-'0')+COMPILED_DATE[10]-'0';
	hour = 10*(COMPILED_TIME[0]-'0')+COMPILED_TIME[1]-'0';
	min = 10*(COMPILED_TIME[3]-'0')+COMPILED_TIME[4]-'0';
	sec = 10*(COMPILED_TIME[6]-'0')+COMPILED_TIME[7]-'0';
	RtcSet(year, mon, date, hour, min, sec);
}


