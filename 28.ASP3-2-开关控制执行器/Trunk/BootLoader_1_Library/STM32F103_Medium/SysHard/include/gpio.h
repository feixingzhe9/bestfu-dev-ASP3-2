/***************************Copyright bestfu 2015-12-30**************************
**  文    件: gpio.h
**  功    能：stm32f10x GPIO驱动头文件
**  编    译：Keil uVision5 V5.12
**  版    本：V1.0.0
**  编    写：Jay
**  创建日期：2015.12.29
**  修改日期：无
**  说    明：初版
*******************************************************************************/
#ifndef __GPIO_H
#define __GPIO_H

/********************************GPIO PIN 定义*********************************/
#define GPIO_Pin_0                 ((BF_INT16U)0x0001)  /* Pin 0 selected */
#define GPIO_Pin_1                 ((BF_INT16U)0x0002)  /* Pin 1 selected */
#define GPIO_Pin_2                 ((BF_INT16U)0x0004)  /* Pin 2 selected */
#define GPIO_Pin_3                 ((BF_INT16U)0x0008)  /* Pin 3 selected */
#define GPIO_Pin_4                 ((BF_INT16U)0x0010)  /* Pin 4 selected */
#define GPIO_Pin_5                 ((BF_INT16U)0x0020)  /* Pin 5 selected */
#define GPIO_Pin_6                 ((BF_INT16U)0x0040)  /* Pin 6 selected */
#define GPIO_Pin_7                 ((BF_INT16U)0x0080)  /* Pin 7 selected */
#define GPIO_Pin_8                 ((BF_INT16U)0x0100)  /* Pin 8 selected */
#define GPIO_Pin_9                 ((BF_INT16U)0x0200)  /* Pin 9 selected */
#define GPIO_Pin_10                ((BF_INT16U)0x0400)  /* Pin 10 selected */
#define GPIO_Pin_11                ((BF_INT16U)0x0800)  /* Pin 11 selected */
#define GPIO_Pin_12                ((BF_INT16U)0x1000)  /* Pin 12 selected */
#define GPIO_Pin_13                ((BF_INT16U)0x2000)  /* Pin 13 selected */
#define GPIO_Pin_14                ((BF_INT16U)0x4000)  /* Pin 14 selected */
#define GPIO_Pin_15                ((BF_INT16U)0x8000)  /* Pin 15 selected */
#define GPIO_Pin_All               ((BF_INT16U)0xFFFF)  /* All pins selected */

/*****************************GPIO 复用选择功能定义****************************/ 
#define GPIO_Remap_SPI1             ((BF_INT32U)0x00000001)  /*!< SPI1 Alternate Function mapping */
#define GPIO_Remap_I2C1             ((BF_INT32U)0x00000002)  /*!< I2C1 Alternate Function mapping */
#define GPIO_Remap_USART1           ((BF_INT32U)0x00000004)  /*!< USART1 Alternate Function mapping */
#define GPIO_Remap_USART2           ((BF_INT32U)0x00000008)  /*!< USART2 Alternate Function mapping */
#define GPIO_PartialRemap_USART3    ((BF_INT32U)0x00140010)  /*!< USART3 Partial Alternate Function mapping */
#define GPIO_FullRemap_USART3       ((BF_INT32U)0x00140030)  /*!< USART3 Full Alternate Function mapping */
#define GPIO_PartialRemap_TIM1      ((BF_INT32U)0x00160040)  /*!< TIM1 Partial Alternate Function mapping */
#define GPIO_FullRemap_TIM1         ((BF_INT32U)0x001600C0)  /*!< TIM1 Full Alternate Function mapping */
#define GPIO_PartialRemap1_TIM2     ((BF_INT32U)0x00180100)  /*!< TIM2 Partial1 Alternate Function mapping */
#define GPIO_PartialRemap2_TIM2     ((BF_INT32U)0x00180200)  /*!< TIM2 Partial2 Alternate Function mapping */
#define GPIO_FullRemap_TIM2         ((BF_INT32U)0x00180300)  /*!< TIM2 Full Alternate Function mapping */
#define GPIO_PartialRemap_TIM3      ((BF_INT32U)0x001A0800)  /*!< TIM3 Partial Alternate Function mapping */
#define GPIO_FullRemap_TIM3         ((BF_INT32U)0x001A0C00)  /*!< TIM3 Full Alternate Function mapping */
#define GPIO_Remap_TIM4             ((BF_INT32U)0x00001000)  /*!< TIM4 Alternate Function mapping */
#define GPIO_Remap1_CAN1            ((BF_INT32U)0x001D4000)  /*!< CAN1 Alternate Function mapping */
#define GPIO_Remap2_CAN1            ((BF_INT32U)0x001D6000)  /*!< CAN1 Alternate Function mapping */
#define GPIO_Remap_PD01             ((BF_INT32U)0x00008000)  /*!< PD01 Alternate Function mapping */
#define GPIO_Remap_TIM5CH4_LSI      ((BF_INT32U)0x00200001)  /*!< LSI connected to TIM5 Channel4 input capture for calibration */
#define GPIO_Remap_ADC1_ETRGINJ     ((BF_INT32U)0x00200002)  /*!< ADC1 External Trigger Injected Conversion remapping */
#define GPIO_Remap_ADC1_ETRGREG     ((BF_INT32U)0x00200004)  /*!< ADC1 External Trigger Regular Conversion remapping */
#define GPIO_Remap_ADC2_ETRGINJ     ((BF_INT32U)0x00200008)  /*!< ADC2 External Trigger Injected Conversion remapping */
#define GPIO_Remap_ADC2_ETRGREG     ((BF_INT32U)0x00200010)  /*!< ADC2 External Trigger Regular Conversion remapping */
#define GPIO_Remap_ETH              ((BF_INT32U)0x00200020)  /*!< Ethernet remapping (only for Connectivity line devices) */
#define GPIO_Remap_CAN2             ((BF_INT32U)0x00200040)  /*!< CAN2 remapping (only for Connectivity line devices) */
#define GPIO_Remap_SWJ_NoJTRST      ((BF_INT32U)0x00300100)  /*!< Full SWJ Enabled (JTAG-DP + SW-DP) but without JTRST */
#define GPIO_Remap_SWJ_JTAGDisable  ((BF_INT32U)0x00300200)  /*!< JTAG-DP Disabled and SW-DP Enabled */
#define GPIO_Remap_SWJ_Disable      ((BF_INT32U)0x00300400)  /*!< Full SWJ Disabled (JTAG-DP + SW-DP) */
#define GPIO_Remap_SPI3             ((BF_INT32U)0x00201100)  /*!< SPI3/I2S3 Alternate Function mapping (only for Connectivity line devices) */
#define GPIO_Remap_TIM2ITR1_PTP_SOF ((BF_INT32U)0x00202000)  /*!< Ethernet PTP output or USB OTG SOF (Start of Frame) connected
                                                                 to TIM2 Internal Trigger 1 for calibration
                                                                 (only for Connectivity line devices) */
#define GPIO_Remap_PTP_PPS          ((BF_INT32U)0x00204000)  /*!< Ethernet MAC PPS_PTS output on PB05 (only for Connectivity line devices) */

#define GPIO_Remap_TIM15            ((BF_INT32U)0x80000001)  /*!< TIM15 Alternate Function mapping (only for Value line devices) */
#define GPIO_Remap_TIM16            ((BF_INT32U)0x80000002)  /*!< TIM16 Alternate Function mapping (only for Value line devices) */
#define GPIO_Remap_TIM17            ((BF_INT32U)0x80000004)  /*!< TIM17 Alternate Function mapping (only for Value line devices) */
#define GPIO_Remap_CEC              ((BF_INT32U)0x80000008)  /*!< CEC Alternate Function mapping (only for Value line devices) */
#define GPIO_Remap_TIM1_DMA         ((BF_INT32U)0x80000010)  /*!< TIM1 DMA requests mapping (only for Value line devices) */

#define GPIO_Remap_TIM9             ((BF_INT32U)0x80000020)  /*!< TIM9 Alternate Function mapping (only for XL-density devices) */
#define GPIO_Remap_TIM10            ((BF_INT32U)0x80000040)  /*!< TIM10 Alternate Function mapping (only for XL-density devices) */
#define GPIO_Remap_TIM11            ((BF_INT32U)0x80000080)  /*!< TIM11 Alternate Function mapping (only for XL-density devices) */
#define GPIO_Remap_TIM13            ((BF_INT32U)0x80000100)  /*!< TIM13 Alternate Function mapping (only for High density Value line and XL-density devices) */
#define GPIO_Remap_TIM14            ((BF_INT32U)0x80000200)  /*!< TIM14 Alternate Function mapping (only for High density Value line and XL-density devices) */
#define GPIO_Remap_FSMC_NADV        ((BF_INT32U)0x80000400)  /*!< FSMC_NADV Alternate Function mapping (only for High density Value line and XL-density devices) */

#define GPIO_Remap_TIM67_DAC_DMA    ((BF_INT32U)0x80000800)  /*!< TIM6/TIM7 and DAC DMA requests remapping (only for High density Value line devices) */
#define GPIO_Remap_TIM12            ((BF_INT32U)0x80001000)  /*!< TIM12 Alternate Function mapping (only for High density Value line devices) */
#define GPIO_Remap_MISC             ((BF_INT32U)0x80002000)  /*!< Miscellaneous Remap (DMA2 Channel5 Position and DAC Trigger remapping, 
                                                                 only for High density Value line devices) */ 

/*************************************自定义宏********************************/
#define ENABLE						((BF_INT08U)(0x01))		/*!< 表示使能 */ 
#define DISENABLE					((BF_INT08U)(0x00))		/*!< 表示禁能 */ 

/********************************数据结构定义*********************************/
typedef enum{
	GPIO_A = 0,
	GPIO_B = 1,
	GPIO_C = 2,
	GPIO_D = 3,
	GPIO_E = 4,
	GPIO_F = 5,
	GPIO_G = 6
}GPIO_Port;  //GPIO端口定义

typedef enum
{
	AN_IN			= 0x00,		/* 模拟输入 */
	IN_FLOATING 	= 0x04,		/* 浮空输入 */	
	IN_UP   		= 0x18,		/* 上拉输入 */
	IN_DOWN 		= 0x08,		/* 下拉输入 */	
	OUT_PP_2M		= 0x02,		/* 通用推挽输出 */
	OUT_PP_10M		= 0x01,
	OUT_PP_50M		= 0x03,	
	OUT_OP_2M		= 0x06,		/* 开漏输出 */
	OUT_OP_10M		= 0x05,
	OUT_OP_50M		= 0x07,	
	AF_PP_2M		= 0x0a,		/* 复用推挽输出 */
	AF_PP_10M		= 0x09,
	AF_PP_50M		= 0x0b,	
	AF_OP_2M		= 0x0e,		/* 复用开漏输出 */
	AF_OP_10M		= 0x0d,
	AF_OP_50M		= 0x0f
}GPIO_Mode;  //GPIO模式定义
/*********************************函数声明**************************************/
void GPIO_Config(GPIO_Port GPIO_x,BF_INT16U GPIO_Pin,GPIO_Mode mode);
void GPIO_SetBits(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin);
void GPIO_ResetBits(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin);
void GPIO_ToggleBits(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin);
BF_INT08U GPIO_ReadOutputDataBit(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin);
BF_INT08U GPIO_ReadInputDataBit(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin);
void GPIO_PinLockConfig(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin);
void GPIO_PinRemapConfig(BF_INT32U GPIO_Remap, BF_INT08U NewState);

#endif

