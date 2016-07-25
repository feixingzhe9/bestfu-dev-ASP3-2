/***************************Copyright BestFu ***********************************
**  文    件:   relay.c
**  功    能：  继电器操作相关驱动层函数
**  编    译：  Keil uVision5 V5.12
**  版    本：  V1.0.0
**  编    写：  Jay
**  创建日期：  2015/08/10
**  修改日期：  2015/08/10
**  说    明：
**  版     本： V1.0
*******************************************************************************/
#include "relay.h"
#include "sys.h"
/******************************************************************************/
u8 CPU_CntLeadZeros(u32 val);
/*******************************************************************************
**函    数： void GPIO_Init(pCheckPort_t	pCheckPort,pControlPort_t	pControlPort)
**功    能： GPIO初始化
**参    数： pCheckPort:待初始化的检查PIN IO口结构体指针;pControlPort:待初始化的控制PIN IO口结构体指针;
**返    回： void
**说    明： 无
*******************************************************************************/
void GPIO_Init(pCheckPort_t	pCheckPort,pControlPort_t	pControlPort)
{
	if(NULL	!=	pCheckPort)
	{
		GPIOx_Cfg(pCheckPort->CheckPort,pCheckPort->CheckPin, IN_UPDOWN);                   /**< 配置过零参考IO为上拉输入 */
		GPIOx_Set(pCheckPort->CheckPort,pCheckPort->CheckPin);
	}
	
	if(NULL	!=	pControlPort)
	{
		GPIOx_Cfg(pControlPort->ControlPort,pControlPort->ControlPin,OUT_PP_50M);          /**< 配置单元继电器控制脚IO为通用推挽输出 */
		GPIOx_Rst(pControlPort->ControlPort,pControlPort->ControlPin);                     /**< 配置单元继电器控制脚IO状态初始化为0  */
	}
}

/*******************************************************************************
**函    数： void EXIT_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,pCheckPort_t	pCheckPort)	
**功    能： 外部中断初始化
**参    数： NVIC_PreemptionPriority:抢占优先级
             NVIC_SubPriority       :响应优先级
             NVIC_Channel           :中断编号
             pCheckPort				      :待初始化的检查PIN IO口结构体指针;
**返    回： void
**说    明： 无
*******************************************************************************/
void EXIT_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,pCheckPort_t	pCheckPort)	
{	
	u8 BIT = (u8)(31-CPU_CntLeadZeros((u32)(pCheckPort->CheckPin)));
	MY_NVIC_Init(NVIC_PreemptionPriority,                                               /**> 设置NVIC 				**/
                 NVIC_SubPriority,
                ((u8)NVIC_Channel),2);
	Ex_NVIC_Config(	pCheckPort->CheckPort,		                                        /**<  下降沿触发        	**/
                  BIT,
                  FTIR);              
	EXTI->IMR&=~((u32)(pCheckPort->CheckPin));			                                /**<  关闭line2上的中断 	**/
	EXTI->PR |= ((u32)(pCheckPort->CheckPin));
}

/*******************************************************************************
**函    数： void TIM_Init(TIM_TypeDef	*TIM)
**功    能： 定时器初始化
**参    数： NVIC_PreemptionPriority:抢占优先级
             NVIC_SubPriority       :响应优先级
             NVIC_Channel           :中断编号
             TIM					          :待初始化定时器
**返    回： void
**说    明： 无
*******************************************************************************/
void TIM_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,TIM_TypeDef  *TIM)
{
	if(NULL == TIM) return;
	if(TIM1 == TIM)       RCC->APB2ENR	|= (1u << 11);                                      /**> 初始化定时器 **/
	else if(TIM2 == TIM)  RCC->APB1ENR	|= (1u << 0);
	else if(TIM3 == TIM)  RCC->APB1ENR	|= (1u << 1);
	else if(TIM4 == TIM)  RCC->APB1ENR	|= (1u << 2);
	else if(TIM5 == TIM)  RCC->APB1ENR	|= (1u << 3);
	else if(TIM6 == TIM)  RCC->APB1ENR	|= (1u << 4);
	else if(TIM7 == TIM)  RCC->APB1ENR	|= (1u << 5);
	else                  RCC->APB2ENR	|= (1u << 13);
 	TIM->ARR                    = 50000;                                                    /**< 设定计数器自动重装值              	**/
	TIM->PSC                    = 71;                                                       /**< 预分频器72,得到1Mhz的计数时钟     	**/
	TIM->CR1                   &=~(1<<0);                                                   /**< 失能定时器	                     	**/  
	MY_NVIC_Init(	NVIC_PreemptionPriority,	                                                /**< 抢占1，子优先级3，组2             	**/
                NVIC_SubPriority,
                ((u8)NVIC_Channel),2);             
	TIM->SR                    &=~(1<<0);                                                   /**< 清定时器溢出标志位             	**/
	TIM->DIER                  &=~(1<<0);                                                   /**< 禁止更新中断                      	**/
}

/*******************************************************************************
**函    数： __asm static  u8 CPU_CntLeadZeros(u32 val)	
**功    能： 计算前导零
**参    数： val:待计算前导零的u32数
**返    回： 前导零的个数
**说    明： 无
*******************************************************************************/
__asm  u8 CPU_CntLeadZeros(u32 val)
{
    CLZ     R0, R0                                                                          /**> Count leading zeros  **/
    BX      LR
}

/********************************End of File***********************************/
