/***************************Copyright xxxxx 2015-12-29*************************
**  文	件：system_stm32f10x.h
**  说	明：系统时钟初始化相关源代码头文件
**  编	译：uVision V5.12.0.0
**  版	本：V1.0
**  编	写：Jay
**  日	期：2015/12/29
**  修　改：暂无
*******************************************************************************/
#ifndef __SYSTEM_STM32F10X_H
#define __SYSTEM_STM32F10X_H

#ifdef __cplusplus
 extern "C" {
#endif 


extern void SystemInit(void);
extern unsigned char  Get_SysClk_FrqAndSrc (unsigned int *frq,unsigned char *src);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /*__SYSTEM_STM32F10X_H */
  
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
