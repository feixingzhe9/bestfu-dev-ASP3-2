/***************************Copyright BestFu 2015-11-02*************************
文 件:	KeyUnit.h
说 明:	语音播报设备属性实现文件
编 辑:	Keil uVision5 V5.12
版 本:	v1.0
编 写:	lsc
日 期:	2015.11.02
修 改:	暂无
*******************************************************************************/
#ifndef __KEYUNIT_H
#define __KEYUNIT_H

#include "BF_type.h"
#include "WTC6104BSI.h"

/*****************************************************************************/
#define KEY_UNIT                (0x03)      /**> 按键所在的单元号           **/
#define KEY1_ACTION_CMD         (0x01)      /**> 按键1状态资源所在的属性号  **/
#define KEY1_STATE_CMD          (0x02)      /**> 按键1状态资源所在的属性号  **/

#define KEY2_ACTION_CMD         (0x03)      /**> 按键2状态资源所在的属性号  **/
#define KEY2_STATE_CMD          (0x04)      /**> 按键2状态资源所在的属性号  **/

#define KEY3_ACTION_CMD         (0x05)      /**> 按键3状态资源所在的属性号  **/
#define KEY3_STATE_CMD          (0x06)      /**> 按键3状态资源所在的属性号  **/

/*******************************按键IO宏定义*********************************/
#define PORT_GPIO_KEY_1     	GPIO_A
#define PIN_GPIO_KEY_1      	pin_6		/**> 按键1                      **/

#define PORT_GPIO_KEY_2     	GPIO_A
#define PIN_GPIO_KEY_2      	pin_11		/**> 按键2       				**/

#define PORT_GPIO_KEY_3     	GPIO_A
#define PIN_GPIO_KEY_3      	pin_5		/**> 按键3       				**/

/*******************************定义EEPROM中的存储位置***********************/
#define SW_BINDING_ONE_ADDR	    (0x3208)	/**> 开关执行器绑定状态存储地址   **/
#define SW_BINDING_TWO_ADDR	    (0x320c)	/**> 开关执行器绑定状态存储地址   **/

/*****************************************************************************/
typedef enum
{
    KEY_NONE  = 0x00,
    KEY_SHORT = 0x01,
    KEY_LONG  = 0x02
}KeyState_e;

typedef enum
{
    SWITCH_OFF =0x00,
    SWITCH_ON = 0x01
}SWState_e;

/**********************************变量声明*********************************/
extern Key_OPT_t   Key_OPT[3];
/*****************************************************************************/
void Key_Handle(void);
void Key_Unit_Init(void);
/*****************************************************************************/
#endif
