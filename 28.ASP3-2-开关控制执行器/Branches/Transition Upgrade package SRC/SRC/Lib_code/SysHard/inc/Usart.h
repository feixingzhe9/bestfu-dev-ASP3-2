/***************************Copyright BestFu 2014-05-14*************************
文	件：	UART.h
说	明：	串口操作相关函数
编	译：	Keil uVision4 V4.54.0.0
版	本：	v1.0
编	写：	Unarty
日	期：	2014.07.03 
修　改：	暂无
*******************************************************************************/
#ifndef __USART_H
#define __USART_H

#include "BF_type.h"

void UART_Init(u32 pclk2, u32 bound);
u8 UART_Write(u8 c);
u8 UART_Send(u8 *data, u8 len, u8 level);

#endif	   
/**************************Copyright BestFu 2014-05-14*************************/
