/***************************Copyright xxxxx 2015-12-29*************************
**  文	  件ys.h
**  说	  明：系统公用处理函数头文件
**  编	  译：uVision V5.12.0.0
**  版	  本：V1.0
**  编	  写：Jay
**  创建日期：2015/12/29
**  修改日期：无
**  说    明：初版
*******************************************************************************/
#ifndef __SYS_H
#define __SYS_H	

//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

/*********************************JTAG模式定义**********************************/
#define JTAG_SWD_DISABLE   (0X02)
#define SWD_ENABLE         (0X01)
#define JTAG_SWD_ENABLE    (0X00)

/******************************GPIO中断PORT口定义*******************************/
#define EXTI_PortSourceGPIOA       ((BF_INT08U)0x00)
#define EXTI_PortSourceGPIOB       ((BF_INT08U)0x01)
#define EXTI_PortSourceGPIOC       ((BF_INT08U)0x02)
#define EXTI_PortSourceGPIOD       ((BF_INT08U)0x03)
#define EXTI_PortSourceGPIOE       ((BF_INT08U)0x04)
#define EXTI_PortSourceGPIOF       ((BF_INT08U)0x05)
#define EXTI_PortSourceGPIOG       ((BF_INT08U)0x06)
#define EXTI_PortSourceGPIOH       ((BF_INT08U)0x07)
#define EXTI_PortSourceGPIOI       ((BF_INT08U)0x08)

/******************************GPIO中断Pin口定义********************************/
#define EXTI_PinSource0            ((BF_INT08U)0x00)
#define EXTI_PinSource1            ((BF_INT08U)0x01)
#define EXTI_PinSource2            ((BF_INT08U)0x02)
#define EXTI_PinSource3            ((BF_INT08U)0x03)
#define EXTI_PinSource4            ((BF_INT08U)0x04)
#define EXTI_PinSource5            ((BF_INT08U)0x05)
#define EXTI_PinSource6            ((BF_INT08U)0x06)
#define EXTI_PinSource7            ((BF_INT08U)0x07)
#define EXTI_PinSource8            ((BF_INT08U)0x08)
#define EXTI_PinSource9            ((BF_INT08U)0x09)
#define EXTI_PinSource10           ((BF_INT08U)0x0A)
#define EXTI_PinSource11           ((BF_INT08U)0x0B)
#define EXTI_PinSource12           ((BF_INT08U)0x0C)
#define EXTI_PinSource13           ((BF_INT08U)0x0D)
#define EXTI_PinSource14           ((BF_INT08U)0x0E)
#define EXTI_PinSource15           ((BF_INT08U)0x0F)

/********************************中断分组参量定义******************************/
#define NVIC_PriorityGroup_0         ((BF_INT08U)0x00) 
#define NVIC_PriorityGroup_1         ((BF_INT08U)0x01) 
#define NVIC_PriorityGroup_2         ((BF_INT08U)0x02) 
#define NVIC_PriorityGroup_3         ((BF_INT08U)0x03) 
#define NVIC_PriorityGroup_4         ((BF_INT08U)0x04) 

/********************************中断触发方式**********************************/
#define EXTI_Trigger_Falling		 ((BF_INT08U)(0x01))
#define EXTI_Trigger_Rising			 ((BF_INT08U)(0x02))
#define EXTI_Trigger_Rising_Falling  ((BF_INT08U)(0x03))

/**********************************函数声明************************************/ 
void MY_NVIC_SetVectorTable(BF_INT32U NVIC_VectTab, BF_INT32U Offset);
void MY_NVIC_PriorityGroupConfig(BF_INT08U NVIC_Group);
void MY_NVIC_Init(BF_INT08U NVIC_PreemptionPriority,BF_INT08U NVIC_SubPriority,BF_INT08U NVIC_IRQChannel,BF_INT08U NVIC_Group);
void EXTI_NVIC_Config(BF_INT08U GPIOx,BF_INT08U BITx,BF_INT08U TRIM);
void JumpToCode(BF_INT32U addr);

#endif
/**********************************End of File*********************************/
