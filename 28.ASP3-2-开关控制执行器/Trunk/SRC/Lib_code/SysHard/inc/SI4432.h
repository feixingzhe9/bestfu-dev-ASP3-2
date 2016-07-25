/***************************Copyright BestFu 2014-05-14*************************
��	����    SI4432.h
˵	����    SI4432����ͷ�ļ�
��	�룺    Keil uVision4 V4.54.0.0
��	����    v1.0
��	д��    Unarty
��	�ڣ�    2013-09-30
�ޡ��ģ�	2014.07.23	Seven  amend	���ݾɿ�
*******************************************************************************/
#ifndef __SI4432_H
#define __SI4432_H

#include "BF_type.h"


//ע��sts��ֵֻ��Ϊ1��0
#define SDN(sts)    STATUS##sts(SDN_PORT, SDN_PIN)
#define nSEL(sts)   STATUS##sts(nSEL_PORT, nSEL_PIN)

#define nIRQ        GPIOx_Get(nIRQ_PORT, nIRQ_PIN)
#define nGPIO2      GPIOx_Get(nGPIO2_PORT, nGPIO2_PIN)

#define R_REG       0x00    //���Ĵ���
#define W_REG       0x80    //д�Ĵ���

#define MAX_PAGE	0xff	//�����243���ֽڣ�������255���ܳ����ڲ�ĳЩͷβ�ж�u8������Ӱ�죬�������Ϊ243

typedef enum
{
	SI4432_TXS 		= (1<<0),		//����״̬��ʶ
	SI4432_RXS 		= (1<<1),		//����״̬��ʶ
	SI4432_TXF  	= (1<<3),		//���ݷ��ʹ���
	SI4432_RXF  	= (1<<4),		//���ݽ��մ���
	SI4432_CRCF  	= (1<<5),		//����CRC����
	SI4432_BUSYF 	= (1<<6),		//�ŵ�æ����
	SI4432_RSSIF	= (1<<7)		//ģ��ͨ�Ŵ���
}SI4432State_t;


void Si4432_Init(void);
void Si4432_Idle(void);
u8 Si4432_Send(u8 *cptBuf, u8 cLength, u8 level);
extern u32 Get_Sign433_MasterAddr(void);//��ȡ����������ַ
extern u8  Get_Sign433_TestPackNum(void);//��ȡ���Ա�����
#endif //si4432.h
/**************************Copyright BestFu 2014-05-14*************************/

