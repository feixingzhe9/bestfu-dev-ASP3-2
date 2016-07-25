/***************************Copyright BestFu ***********************************
**  文    件:   relay.h
**  功    能：  继电器操作相关驱动层函数头文件
**  编    译：  Keil uVision5 V5.12
**  版    本：  V1.0.0
**  编    写：  Jay
**  创建日期：  2015/08/10
**  修改日期：  2015/08/10
**  说    明：
**  版     本： V1.0
*******************************************************************************/
#ifndef __RELAY_H
#define __RELAY_H

#include "BF_type.h"
#include "stm32f10x.h"
#include "GPIO.h"

/********************************GPIO定义*************************************/
#define RELAY_ONE_PORT          GPIO_A      /**< 两路开关IO口                     */
#define RELAY_ONE_CTR_PIN 	    pin_4       /**< 两路开关控制脚                   */
#define RELAY_ONE_CHK_PIN 	    pin_1       /**< 两路开关过零检测脚               */

#define RELAY_TWO_PORT          GPIO_A      /**< 两路开关IO口                     */
#define RELAY_TWO_CTR_PIN 	    pin_3       /**< 两路开关控制脚                   */
#define RELAY_TWO_CHK_PIN 	    pin_0       /**< 两路开关过零检测脚               */

#define AC_ZERO_REF_PORT        GPIO_A      /**< 两路开关IO口                     */
#define AC_ZERO_REF_CHK_PIN     pin_2       /**< 两路开关过零检测脚               */

/*****************************************************************************/
#define EXTI_IRQHandler(x)		EXTI##x_IRQHandler
/********************************数据结构定义*********************************/
typedef struct			                        /**> 用于检测过零点时刻和验证过零点 	**/
{
	GPIO_Port	CheckPort;
	u16			CheckPin;
}CheckPort_t,*pCheckPort_t;


typedef struct			                        /**> 用于设置继电器的开关 						**/
{
	GPIO_Port	ControlPort;
	u16			ControlPin;
}ControlPort_t,*pControlPort_t;

/*****************************************************************************/
void GPIO_Init(pCheckPort_t	pCheckPort,pControlPort_t	pControlPort);
void EXIT_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,pCheckPort_t	pCheckPort);
void TIM_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,TIM_TypeDef *TIM);
/*****************************************************************************/
#endif

