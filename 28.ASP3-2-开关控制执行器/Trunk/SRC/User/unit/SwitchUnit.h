/***************************Copyright BestFu 2015-08-03*************************
文 件:	SwitchUnit.h
说 明:	执行器设备属性实现文件
编 辑:	Keil uVision5 V5.12
版 本:	v1.0
编 写:	Jay
日 期:	2015.08.11
修 改:	暂无
*******************************************************************************/
#ifndef __SWITCHUNIT_H
#define __SWITCHUNIT_H
/******************************************************************************/
#include "BF_type.h"
#include "WTC6104BSI.h"
#include "KeyUnit.h"
#include "relay.h"

/*****************************************************************************/

//#define offsetof(TYPE, MEMBER) ((u32) &((TYPE *)0)->MEMBER)     /**> 用于获取结构体中成员的偏移量 **/

/*******************************定义EEPROM中的存储位置***********************/
#define SW_POWERON_ONE_ADDR	              (0x3200)	            /**> 开关执行器上电状态存储地址   **/
#define SW_POWERON_TWO_ADDR	              (0x3204)	            /**> 开关执行器上电状态存储地址   **/

#define ADDR_CROSS_ZERO_UNIT_ONE_ON       (0x4000)              /**> 开关执行器开过零补偿时间存储地址**/
#define ADDR_CROSS_ZERO_UNIT_ONE_OFF      (0x4004)              /**> 开关执行器关过零补偿时间存储地址**/
#define ADDR_CROSS_ZERO_UNIT_TWO_ON       (0x4008)              /**> 开关执行器开过零补偿时间存储地址**/
#define ADDR_CROSS_ZERO_UNIT_TWO_OFF      (0x400c)              /**> 开关执行器关过零补偿时间存储地址**/
#define AC_FRE_VALUE                      (19000)               /**> 开关执行器上电状态存储地址      **/
/***************************继电器过零点误差限********************************/

#define RELAY_CROSSZERO_ON_UPPER_LIMIT    ((AC_FRE_VALUE/2))    /**> 用于初始化时判断是否需要自动校零 **/
#define RELAY_CROSSZERO_ON_LOWER_LIMIT    (4000)

#define RELAY_CROSSZERO_OFF_UPPER_LIMIT   (4500)
#define RELAY_CROSSZERO_OFF_LOWER_LIMIT   (2000)

/****************************继电器过零点标准值*******************************/

#define RELAY_CROSSZERO_ON_TIME            (6200)               /**> 此处是继电器的打开时间 **/
#define RELAY_CROSSZERO_OFF_TIME           (3150)               /**> 此处是继电器的闭合时间 **/

/*************************继电器相关数据类型定义******************************/

typedef enum
{
    SW_OFF = 0X00,                                              /**> 表示继电器关动作 **/
    SW_ON  = 0X01,                                              /**> 表示继电器开动作 **/
}RelayActionType_t;


typedef struct
{
    u16 CrossZeroTimeON;                                        /**> 表示继电器的开合时间 **/
    u16 CrossZeroTimeOFF;                                       /**> 表示继电器的闭合时间 **/
    u8  IRQ_Flag;                                               /**> 表示参考零点中断标志 **/
    u8  CheckFlagON;                                            /**> 表示继电器的校验标志 **/
    u8  CheckFlagOFF;                                           /**> 表示继电器的校验标志 **/
    u8  RelayActionType;                                        /**> 表示继电器动作类型   **/
}SW_CrossZero_t,*pSW_CrossZero_t;


typedef struct
{
	void (*fun)(void);			                                /**> 开始状态函数指针                 **/
	u32 firstTime;
	u32 secondTime;
	u8  cycleFlag;		                                        /**> 循环标记                         **/
}SwActTime_t;

typedef struct			                                        /**> 参考零点结构体，过零检测公用参考 **/
{
	pCheckPort_t	pCheckPort;		                            /**> 定义GPIO pin 					  **/
}Ref_Zero_t,*pRef_Zero_t;

typedef struct										            /**> 参考零点结构体，过零检测公用参考 **/
{
    pCheckPort_t	pCheckPort;						            /**> 定义GPIO 检查pin 		          **/
    pControlPort_t	pControlPort;					            /**> 定义GPIO 控制pin 			      **/
    TIM_TypeDef 	*TIM;							            /**> TIM初始化结构体                  **/
    u16             CrossZeroEEPROMAddr;                        /**> 过零检测记录                     **/
    SW_CrossZero_t  SW_CrossZero;                               /**> 过零检测参数集合                 **/
    SwActTime_t     SwActTime;                                  /**> 用于开关计划函数                 **/
    u16             PowerOnEEPROMAddr;                          /**> 上电状态记录                     **/
    u8              PowerOnState;                               /**> 上电状态                         **/
    u8              TIM_First_InitFlag;                         /**> 定时器首次使用标志               **/
    void            (*SW_ON)(void);                             /**> 开关执行器开函数指针             **/
    void            (*SW_OFF)(void);                            /**> 开关执行器关函数指针             **/
    void            (*SW_REV)(void);                            /**> 开关执行器取反函数指针           **/
    void            (*SW_DelayOpt1)(void);                      /**> 延时处理函数1函数指针,用于开关计划**/
    void            (*SW_DelayOpt2)(void);                      /**> 延时处理函数2函数指针,用于开关计划**/
    void            (*SW_DelayClr)(void);                       /**> 动作清除函数指针                 **/
}Sw_Act_Base_t,*pSw_Act_Base_t;

typedef struct                                                  /**> 开关执行器结构体                 **/
{
    Ref_Zero_t  Ref_Zero;                                       /**> 公用参考零点                     **/
    Sw_Act_Base_t Sw_Act_Base[2];   
}Switch_Actuator_t,*pSwitch_Actuator_t;

/*********************************数据变量声明*********************************/
extern Switch_Actuator_t   Switch_Actuator;     /**> 开关执行器结构体定义 **/

/*********************************函数声明************************************/
void Switch_ACZERO_REF_Unit(void);
void Switch_ONE_Unit_Init(void);
void Switch_TWO_Unit_Init(void);
void SW_UNIT_ONE_Reverse(void);
void SW_UNIT_ONE_ON(void);
void SW_UNIT_ONE_OFF(void);
void SW_UNIT_TWO_ON(void);
void SW_UNIT_TWO_OFF(void);
void SW_UNIT_TWO_Reverse(void);
void SW_UNIT_ONE_DelayClr(void);
void SW_UNIT_TWO_DelayClr(void);

void SW_TWO_DelayOpt1(void);
void SW_TWO_DelayOpt2(void);

void SW_ONE_DelayOpt1(void);
void SW_ONE_DelayOpt2(void);

/******************************************************************************/
#endif
