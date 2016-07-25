/***************************Copyright xxxxx 2015-12-30**************************
**  文    件：  iwdg.h
**  功    能：  独立看门狗底层驱动函数头文件
**  编    译：  MDK-ARM Professional Versoin: 5.12.0.0
**  版    本：  V1.0
**  编    写：  Jay
**  创建日期：  2015-12-30
**  修改日期：  无
**  说    明：  初版
*******************************************************************************/
#ifndef __IWDG_H
#define __IWDG_H

#ifdef __cplusplus
 extern "C" {
#endif


/********************************自定义宏*************************************/
//defgroup IWDG_prescaler 
#define IWDG_Prescaler_4            ((uint8_t)0x00)
#define IWDG_Prescaler_8            ((uint8_t)0x01)
#define IWDG_Prescaler_16           ((uint8_t)0x02)
#define IWDG_Prescaler_32           ((uint8_t)0x03)
#define IWDG_Prescaler_64           ((uint8_t)0x04)
#define IWDG_Prescaler_128          ((uint8_t)0x05)
#define IWDG_Prescaler_256          ((uint8_t)0x06)


/******************************函数声明***************************************/
void IWDG_Config(BF_INT08U IWDG_Prescaler,BF_INT16U Reload);
void IWDG_Feed(void);
void IWDG_FeedOFF(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_IWDG_H */
/********************************End of File**********************************/
