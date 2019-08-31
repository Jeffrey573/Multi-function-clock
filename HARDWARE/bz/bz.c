#include "bz.h"
void BzInit(void)
{
	RCC->APB2ENR |= 1<<6;
	GPIOE->CRL &= 0xFF0FFFFF;
	GPIOE->CRL |= 0x00300000;
	GPIOE->ODR |= 0x00000000;
}
/**********************
������������quzi�����У��ҵ�music���鶨��ļ�����������װֵ������������quzi�����е�λ��
��ڲ�����qu��music�����ж���ļ�������
���ڲ��������ص���tem������quzi�����е�λ��ֵ
��ע:
***********************/
u8 QuYin(u8 qu)  //�ҵ���Ӧ������Ӧ���õĶ�ʱ��תʱ��
{
	u8 yin, fu, weizhi;
	yin = qu/16; //����λ��ʾ����������������
	fu = qu%16; //����λ��ʾDO,RI,ME,FA,....
	if( yin==1 )  //������ַ
		weizhi = 0;
	else if( yin==2 ) //������ַ
		weizhi = 7;
	else//������ַ
		weizhi = 14;

	weizhi = weizhi + fu - 1;
	return weizhi;	
}




  