/***************************Copyright xxxxx 2015-12-30**************************
**  文    件：  stm32f4xx_iwdg.c
**  功    能：  独立看门狗底层驱动函数
**  编    译：  MDK-ARM Professional Versoin: 5.12.0.0
**  版    本：  V1.0
**  编    写：  Jay
**  创建日期：  2015-11-26
**  修改日期：  无
**  说    明：  初版
*******************************************************************************/
#include "stm32f10x.h"
#include "lib_type.h"
#include "lib_config.h"
#include "SysHard.h"
#include "Thread.h"
#include "gpio.h"
#include "iwdg.h"

/* KR register bit mask */
#define KR_KEY_RELOAD    			((uint16_t)0xAAAA)//Feed dog value
#define KR_KEY_ENABLE    			((uint16_t)0xCCCC)//starts the watchdog
#define IWDG_WriteAccess_Enable     ((uint16_t)0x5555)//enable access to the IWDG_PR and IWDG_RLR registers
#define IWDG_WriteAccess_Disable    ((uint16_t)0x0000)//disable access to the IWDG_PR and IWDG_RLR registers


/*******************************************************************************
函 数 名： void IWDG_Config(BF_INT08U IWDG_Prescaler,BF_INT16U Reload)
功能说明： 配置独立看门函数
参    数： IWDG_Prescaler:预分频值(IWDG_Prescaler_4,IWDG_Prescaler_8,
						  IWDG_Prescaler_16,IWDG_Prescaler_32,IWDG_Prescaler_64,
						  IWDG_Prescaler_128,IWDG_Prescaler_128)
		   Reload:0x0000~0x0fff
返 回 值： 无
说    明:  无
*******************************************************************************/
void IWDG_Config(BF_INT08U IWDG_Prescaler,BF_INT16U Reload)
{
#if (INTERNAL_IWDG_EN > 0u)
	IWDG->KR = IWDG_WriteAccess_Enable;		//使能IWDG_PR和IWDG_RLR寄存器的访问
	IWDG->PR = IWDG_Prescaler;				//设置预分频值
	IWDG->RLR= Reload;						//设置重装载值
	IWDG->KR = KR_KEY_RELOAD;				//启动重装载值
	IWDG->KR = KR_KEY_ENABLE; 				//启动看门狗	
#endif
#if (EXTERNAL_WDG_EN > 0u)
	GPIO_Config(WDG_PORT, WDG_PIN, OUT_PP_2M);
#endif
#if ((INTERNAL_IWDG_EN > 0u)||(EXTERNAL_WDG_EN > 0u))	
	Thread_Login(FOREVER, 0, 800, &IWDG_Feed);	//注册喂硬件看门狗	
#endif
}

/*******************************************************************************
函 数 名： void IWDG_Feed(void)
功能说明： 独立看门狗喂狗函数
参    数： 无
返 回 值： 无
说    明:  无
*******************************************************************************/
void IWDG_Feed(void)
{
#if (INTERNAL_IWDG_EN > 0u)
	IWDG->KR = KR_KEY_RELOAD;
#endif
#if (EXTERNAL_WDG_EN > 0u)
	GPIO_ToggleBits(WDG_PORT, WDG_PIN);//喂硬件狗
#endif
}

/*******************************************************************************
函 数 名： void IWDG_FeedOFF(void)
功能说明： 关闭独立看门狗喂狗函数
参    数： 无
返 回 值： 无
说    明:  无
*******************************************************************************/
void IWDG_FeedOFF(void)
{
#if ((INTERNAL_IWDG_EN > 0u)||(EXTERNAL_WDG_EN > 0u))	
	Thread_Logout(&IWDG_Feed);
#endif
}

/**************************************End of File*****************************/
