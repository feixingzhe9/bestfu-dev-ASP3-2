/***************************Copyright BestFu 2015-08-03*************************
文 件:	LedUnit.h
说 明:	双路开关执行器设备属性实现文件
编 辑:	Keil uVision5 V5.12
版 本:	v1.0
编 写:	Jay
日 期:	2015.08.03
修 改:	暂无
*******************************************************************************/
#ifndef __LEDUNIT_H
#define __LEDUNIT_H

#include "UnitCfg.h"
#include "led.h"

/*******************************按键IO宏定义*********************************/
#define  PORT_GPIO_LED1     	GPIO_B					/**> led指示灯1 **/
#define  PIN_LED1      			pin_9

#define  PORT_GPIO_LED2     	GPIO_A					/**> led指示灯2 **/
#define  PIN_LED2      			pin_8

#define  PORT_GPIO_LED3     	GPIO_B					/**> led指示灯3 **/
#define  PIN_LED3		      	pin_0

/*******************************数据类型定义********************************/
typedef enum
{
	LED_1_ON,
	LED_2_ON,
	LED_3_ON,
	LED_12_ON,
	LED_13_ON,
	LED_23_ON,
	LED_ALL_ON
}LED_ON_t;

typedef enum
{
	LED_1_OFF = 1,
	LED_2_OFF,
	LED_3_OFF,
	LED_12_OFF,
	LED_13_OFF,
	LED_23_OFF,
	LED_ALL_OFF,
	LED_NULL
}LED_OFF_t;

/*********************************变量声明***********************************/
extern LED_OPT_t LED_OPT[3];

/*********************************函数声明***********************************/
void LED_UNIT_Init(void);
void LED_Flash_Func(LED_ON_t led_on_opt,u8 keynum,LED_OFF_t led_off);
void LED_RVRS_OPT_Func(void);
/****************************************************************************/
#endif
