/*********************������ͷ�ļ�*****************************
* 
***************************************************************/

#ifndef _BZ_H
#define _BZ_H

#include "sys.h"
#include "delay.h"
//����������
#define beep PEout(5)

void BzInit(void);
u8 QuYin(u8 qu);  //�ҵ���Ӧ������Ӧ���õĶ�ʱ��תʱ��
void PlayMusic(u16 song); //���Ŷ�Ӧ�������ַ�
#endif



