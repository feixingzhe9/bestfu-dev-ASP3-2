/***************************Copyright BestFu 2015-08-03*************************
文 件:	Led.h
说 明:	led驱动实现文件
编 辑:	Keil uVision5 V5.12
版 本:	v1.0
编 写:	Jay
日 期:	2015.08.03
修 改:	暂无
*******************************************************************************/
#ifndef _LED_H_   
#define _LED_H_

#include "GPIO.h"

/*****************************************************************************/
typedef void (*LED_Func_PTR)(void);

typedef struct			                        /**> 用于设置LED的开关 						**/
{
	GPIO_Port	ControlPort;
	u16			ControlPin;
}LEDPort_t,*pLEDPort_t;

typedef struct
{
	LED_Func_PTR LED_ON;
	LED_Func_PTR LED_OFF;
	LED_Func_PTR LED_REV;
}LED_Func_PTR_TAB_t,*pLED_Func_PTR_TAB_t;

typedef struct
{
    pLEDPort_t    pLEDPort;
    void (*LED_ON)(void);
    void (*LED_OFF)(void);
    void (*LED_REV)(void);
}LED_OPT_t,*pLED_OPT_t;
    
/****************************************************************************/
void LED_Init(pLEDPort_t pLEDPort);
void LED_ON(pLEDPort_t pLEDPort);
void LED_OFF(pLEDPort_t pLEDPort);
void LED_REV(pLEDPort_t pLEDPort);
/****************************************************************************/
#endif


