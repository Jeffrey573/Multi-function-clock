#include "bz.h"
void BzInit(void)
{
	RCC->APB2ENR |= 1<<6;
	GPIOE->CRL &= 0xFF0FFFFF;
	GPIOE->CRL |= 0x00300000;
	GPIOE->ODR |= 0x00000000;
}
/**********************
功能描述：在quzi数组中，找到music数组定义的简谱音符的重装值，并返回其在quzi数组中的位置
入口参数：qu：music数组中定义的简谱音符
出口参数：返回的是tem音符在quzi数组中的位置值
备注:
***********************/
u8 QuYin(u8 qu)  //找到对应音符的应设置的定时翻转时间
{
	u8 yin, fu, weizhi;
	yin = qu/16; //高四位表示低音、中音、高音
	fu = qu%16; //低四位表示DO,RI,ME,FA,....
	if( yin==1 )  //低音基址
		weizhi = 0;
	else if( yin==2 ) //中音基址
		weizhi = 7;
	else//高音基址
		weizhi = 14;

	weizhi = weizhi + fu - 1;
	return weizhi;	
}




  