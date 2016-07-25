/***************************Copyright BestFu 2014-05-14*************************
��	����	UART.h
˵	����	���ڲ�����غ���
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2014.07.03 
�ޡ��ģ�	����
*******************************************************************************/
#ifndef __USART_H
#define __USART_H

#include "BF_type.h"

void UART_Init(u32 pclk2, u32 bound);
u8 UART_Write(u8 c);
u8 UART_Send(u8 *data, u8 len, u8 level);

#endif	   
/**************************Copyright BestFu 2014-05-14*************************/
