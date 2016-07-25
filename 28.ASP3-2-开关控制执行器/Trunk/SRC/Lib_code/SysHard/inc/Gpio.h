/***************************Copyright BestFu 2014-05-14*************************
��	����	GPIO.h
˵	����	GPIO�˿������ļ�
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2013-08-09
�ޡ��ģ�	����
*******************************************************************************/
#ifndef __GPIO_H
#define __GPIO_H


#include "BF_type.h"


typedef enum{
	GPIO_A = 0,
	GPIO_B = 1,
	GPIO_C = 2,
	GPIO_D = 3,
	GPIO_E = 4,
	GPIO_F = 5,
	GPIO_G = 6
}GPIO_Port;  //GPIO�˿ڶ���

typedef enum{
	pin_0  = (1<<0),
	pin_1  = (1<<1),
	pin_2  = (1<<2),
	pin_3  = (1<<3),
	pin_4  = (1<<4),
	pin_5  = (1<<5),
	pin_6  = (1<<6),
	pin_7  = (1<<7),
	pin_8  = (1<<8),
	pin_9  = (1<<9),
	pin_10 = (1<<10),
	pin_11 = (1<<11),
	pin_12 = (1<<12),
	pin_13 = (1<<13),
	pin_14 = (1<<14),
	pin_15 = (1<<15)
}GPIO_Pin;  //GPIO���Ŷ���

typedef enum{
	IN_         = 0x0,	 //ģ������ģʽ
	IN_UPDOWN   = 0x8,	 //��/��������ģʽ
	IN_FLOATING = 0x4,	 //��������
	OUT_PP_10M  = 0x1,
	OUT_PP_2M   = 0x2,	 //�������
	OUT_PP_50M  = 0x3,
	OUT_OP_10M  = 0x5,
	OUT_OP_2M   = 0x6,	 //��©���
	OUT_OP_50M  = 0x7,
	AF_PP_10M   = 0x9,
	AF_PP_2M    = 0xa,	 //�����������
	AF_PP_50M   = 0xb,
	AF_OP_10M   = 0xd,
	AF_OP_2M    = 0xe,	 //���ÿ�©���
	AF_OP_50M   = 0xf,
	
}GPIO_Mode;  //GPIOģʽ����

#define GPIO_BASE (APB2PERIPH_BASE + 0x0800) //GPIO�˿ڻ�ֵַ


void GPIOx_Cfg(GPIO_Port GPIOx, u16 pin, GPIO_Mode mode);    //GPIO����
u16 GPIOx_Set(GPIO_Port GPIOx, u16 pin);                  //GPIO���������ƽ��1
void GPIOx_Rst(GPIO_Port GPIOx, u16 pin);                    //GPIO���������ƽ��0
u16 GPIOx_Get(GPIO_Port GPIOx, u16 pin);                 	//��ȡGPIO���������ƽ
void GPIOx_Rvrs(GPIO_Port GPIOx, u16 pin);					//GPIO���ŵ�ƽȡ��
//INT32U GPIOx_Lock(GPIO_Port GPIOx, u16 pin, INT8U sta);    //����ӦGPIO������������/ȥ��


#endif //GPIO.h end
/**************************Copyright BestFu 2014-05-14*************************/
