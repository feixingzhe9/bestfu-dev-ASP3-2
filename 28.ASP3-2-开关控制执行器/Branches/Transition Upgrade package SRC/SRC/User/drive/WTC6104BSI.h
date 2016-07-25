/***************************Copyright BestFu 2014-05-14*************************
文 件:	WTC6104BSI.h
说 明:	开关执行器设备驱动实现文件
编 辑:	MDK-ARM Professional Version: 5.12.0.0
版 本:	v1.0
编 写:	Jay
日 期:	2015.11.02
修 改:	暂无
*******************************************************************************/
#ifndef __WTC6104BSI_H
#define __WTC6104BSI_H

#include "UnitCfg.h"
#include "GPIO.h"

/******************************************************************************/

#define KEY_WOBBLE_TIME		   (5)            /**> 按键抖动时间。也就是消抖时间,单位ms                         **/
 
#define KEY_FIXED_PERIOD	   (5)            /**> 固定频率调用按键状态更新函数,括号内为调用周期，周期单位为ms **/

#define KEY_NUM_MAX            (2)            /**> 最大按键数量                                                **/

//#define KEY_SHORT_CNT       (10)            /**> 短按时间计数                                                **/  
#define KEY_LONG_CNT        (200)             /**> 长按时间计数，时间1s                                        **/

/******************************×函数指针定义************************×××××***/


/**************************按键处理相关结构体******************************/
typedef enum
{
	KEY_DOWN 		= 1,                      /**> 按键按下状态     **/
	KEY_UP	 		= 2,                      /**> 按键弹起状态     **/
	KEY_UP_WOBBLE 	= 3,                      /**> 确认弹起消抖状态 **/
	KEY_DOWN_WOBBLE = 4                       /**> 确认按下消抖状态 **/
}KEY_STATE_t;


typedef enum
{
	KEY_NONE_PRESS  = 0,                      /**> 无键按下 **/
	KEY_SHORT_PRESS = 1,                      /**> 短按操作 **/
	KEY_LONG_PRESS	= 2,                      /**> 长按操作 **/
}KEY_TYPE_t;

typedef struct                                /**> 按键动作资源结构体 **/
{
    KEY_TYPE_t  Status;
}KeyAct_t;

typedef struct                                /**> 按键状态资源结构体 **/
{
    KEY_TYPE_t  Status;
}KeySta_t;

typedef struct			                   /**> 用于获取按键状态 			**/
{
	GPIO_Port	CheckPort;
	u16			  CheckPin;
}KeyPort_t,*pKeyPort_t;

typedef struct
{
	pKeyPort_t          pKeyPort;             /**> 按键按键相关结构体                 **/
	u8    (*GetStatePtr)(pKeyPort_t );	      /**> 用于获取按键状态的函数             **/ 
	KEY_STATE_t		    State;                     
	u8		            Time_ms;		      /**> 用于固定周期调用状态更新函数的计时 **/
	KEY_TYPE_t          Key_Type;             /**> 按键类型，长按还是短按             **/ 
	u16                 Key_Count;            /**> 按键持续时间 ，用于长按短按        **/
}KEY_t,*pKey_t;

typedef struct
{
  KEY_t       KEY;
  KeyAct_t    KeyAct;                     /**> 按键的动作资源                **/
  KeySta_t    KeySta;                     /**> 按键的状态资源                **/
  u16         BindingEEPROMAddr;          /**> 绑定关系记录                  **/
  u8          BindingState;               /**> 继电器的绑定状态              **/
  u8          LEDLogicState;              /**> LED的逻辑状态                 **/    
}Key_OPT_t,*pKey_OPT_t;

/****************************变量声明*****************************************/


/****************************************************************************/
void WTC6104BSI_Init(pKey_OPT_t pKey_OPT);
void Key_RefreshState(KEY_t* p_Key);
/****************************************************************************/
#endif
