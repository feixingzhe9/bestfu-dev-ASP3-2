/***************************Copyright xxxxx 2015-12-30**************************
**  文    件：SysTick.c
**  功    能：系统滴答定时器函数
**  编    译：MDK-ARM Professional Versoin: 5.12.0.0
**  版    本：V1.0
**  编    写：Jay
**  创建日期：2015-12-29
**  修改日期：无
**  说    明：初版
*******************************************************************************/
#include "stm32f10x.h"
#include "lib_type.h"
#include "lib_config.h"
#include "Thread.h"
#include "SysTick.h"
#include "system_stm32f10x.h"

//Private macro define 
#define TIME_MAX        (400000000) 			//最大计时时间
#define NVIC_ST_CTRL    (*((volatile BF_INT32U *)0xE000E010))
#define NVIC_ST_RELOAD  (*((volatile BF_INT32U *)0xE000E014))
#define RELOAD_VAL      ((BF_INT32U)(( (BF_INT32U)RCC_Clocks.HCLK_Frequency) / (BF_INT32U)(1000/THREAD_TIME_SLICE)) -1)

//Private variable define 
static volatile BF_INT32U su_timer = 0u;

/*******************************************************************************
函 数 名： void SysTick_Init(void)
功能说明： 系统滴答定时器初始化
参    数： tick:tick重装值
返 回 值： 无
说   明：  无
*******************************************************************************/
void SysTick_Init(void)
{		 
    RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	/*!< Initial System tick.	*/
	NVIC_ST_RELOAD =  RELOAD_VAL; 
    NVIC_ST_CTRL   =  0x0007;
}

/*******************************************************************************
函 数 名：	Time_Get
功能说明： 	获取当前时间
参	  数： 	无
返 回 值：	当前计数值
*******************************************************************************/
BF_INT32U Time_Get(void)
{
    return su_timer;
}

/*******************************************************************************
函 数 名： void SysTick_Handler(void)
功能说明： SysTick中断服务程序
参   数：  无
返 回 值： 无
*******************************************************************************/
void SysTick_Handler(void)
{
	su_timer %= TIME_MAX;	//最大记数1000s
	su_timer++;
	Thread_RunCheck(); 
}
/*********************************************END OF FILE**********************/
