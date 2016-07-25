/***************************Copyright xxxxx 2015-12-29*************************
**  文	件：system_stm32f10x.h
**  说	明：系统时钟初始化相关源代码头文件
**  编	译：uVision V5.12.0.0
**  版	本：V1.0
**  编	写：Jay
**  日	期：2015/12/29
**  修	改：暂无
*******************************************************************************/
#ifndef __SYSTEM_STM32F10X_H
#define __SYSTEM_STM32F10X_H

/*******************************数据结构定义**********************************/
typedef struct
{
  uint32_t SYSCLK_Frequency;  //系统时钟频率
  uint32_t HCLK_Frequency;    //HCLK时钟频率
  uint32_t PCLK1_Frequency;   //PCLK1时钟频率
  uint32_t PCLK2_Frequency;   //PCLK2时钟频率
  uint32_t ADCCLK_Frequency;  //ADC时钟频率
}RCC_ClocksTypeDef;

/*******************************函数声明**************************************/
extern void SystemInit(void);
extern void RCC_GetClocksFreq(RCC_ClocksTypeDef* RCC_Clocks);

#endif /*__SYSTEM_STM32F10X_H */
/**********************************End of File*********************************/
