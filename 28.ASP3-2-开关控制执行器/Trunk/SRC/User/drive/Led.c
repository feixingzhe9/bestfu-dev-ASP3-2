/***************************Copyright BestFu 2015-08-03*************************
文 件:	Led.c
说 明:	led驱动实现文件
编 辑:	Keil uVision5 V5.12
版 本:	v1.0
编 写:	Jay
日 期:	2015.10.08
修 改:	暂无
*******************************************************************************/
#include "led.h"

/*******************************************************************************
函数名  :	void LED_Init(pLEDPort_t pLEDPort)
功能说明：LED端口初始化
参	数  ：pLEDPort:待初始化的GPIO
返回值  ：无
*******************************************************************************/
void LED_Init(pLEDPort_t pLEDPort)
{
    GPIOx_Cfg(pLEDPort->ControlPort,pLEDPort->ControlPin, OUT_PP_50M);
    GPIOx_Rst(pLEDPort->ControlPort,pLEDPort->ControlPin);
}

/*******************************************************************************
函 数 名:  	void LED_ON(pLEDPort_t pLEDPort)
功能说明: 	LED开
参    数:  	pLEDPort:待初始化的GPIO		
返 回 值:  	无
*******************************************************************************/
void LED_ON(pLEDPort_t pLEDPort)
{
    GPIOx_Set(pLEDPort->ControlPort,pLEDPort->ControlPin);
}

/*******************************************************************************
函 数 名:  	void LED_OFF(pLEDPort_t pLEDPort)
功能说明: 	LED关
参    数:  	pLEDPort:待初始化的GPIO		
返 回 值:  	无
*******************************************************************************/
void LED_OFF(pLEDPort_t pLEDPort)
{
    GPIOx_Rst(pLEDPort->ControlPort,pLEDPort->ControlPin);
}

/*******************************************************************************
函 数 名:  	void LED_REV(pLEDPort_t pLEDPort)
功能说明: 	LED取反
参    数:  	pLEDPort:待初始化的GPIO		
返 回 值:  	无
*******************************************************************************/
void LED_REV(pLEDPort_t pLEDPort)
{
    GPIOx_Rvrs(pLEDPort->ControlPort,pLEDPort->ControlPin);
}


/******************************************************************************/
