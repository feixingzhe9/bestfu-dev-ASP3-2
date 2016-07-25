/***************************Copyright BestFu 2015-08-11*************************
文 件:	SwitchUnit.c
说 明:	开关执行器设备属性实现文件
编 辑:	Keil uVision5 V5.12
版 本:	v1.0
编 写:	Jay
日 期:	2015.08.03
修 改:	暂无
*******************************************************************************/

#include "SwitchUnit.h"
#include "sys.h"
#include "EEPROM.h"
#include "Thread.h"
#include "LedUnit.h"
#include "KeyUnit.h"

/*******************************函数变量定义**********************************/
Switch_Actuator_t   Switch_Actuator;     /**> 开关执行器结构体定义 **/

CheckPort_t Ref_Zero[]={AC_ZERO_REF_PORT,AC_ZERO_REF_CHK_PIN};

ControlPort_t Switch_Actuator_Control[2]={{RELAY_ONE_PORT,RELAY_ONE_CTR_PIN},{RELAY_TWO_PORT,RELAY_TWO_CTR_PIN}};
CheckPort_t Switch_Actuator_Check[2]    ={{RELAY_ONE_PORT,RELAY_ONE_CHK_PIN},{RELAY_TWO_PORT,RELAY_TWO_CHK_PIN}};

/*********************************函数声明*************************************/
static void Switch_ON(pRef_Zero_t pRef_Zero,pSw_Act_Base_t pSw_Act_Base,pLED_OPT_t pLED_OPT,pKey_OPT_t pKey_OPT);
static void Switch_OFF(pRef_Zero_t pRef_Zero,pSw_Act_Base_t pSw_Act_Base,pLED_OPT_t pLED_OPT,pKey_OPT_t pKey_OPT);
static void Switch_REV(pRef_Zero_t pRef_Zero,pSw_Act_Base_t pSw_Act_Base,pLED_OPT_t pLED_OPT,pKey_OPT_t pKey_OPT);
static void Ref_OPT_Func(pSw_Act_Base_t pSw_Act_Base);
static void Zero_Check_EXTI_Func(pSw_Act_Base_t pSw_Act_Base);
static void Zero_Check_TIM_Func(pSw_Act_Base_t pSw_Act_Base);
static void Switch_DelayOpt1(pSw_Act_Base_t pSw_Act_Base);
static void Switch_DelayOpt2(pSw_Act_Base_t pSw_Act_Base);
static void Switch_DelayClr(pSw_Act_Base_t pSw_Act_Base);
static void SingleSwitchAttr_Init(pKey_OPT_t pKey_OPT,pSw_Act_Base_t pSw_Act_Base);
/*******************************************************************************
函 数 名:  void Switch_ACZERO_REF_Unit(void)
功能说明:  开关单元一初始化
参    数:  无
返 回 值:  无
*******************************************************************************/
void Switch_ACZERO_REF_Unit(void)
{
  Switch_Actuator.Ref_Zero.pCheckPort  = Ref_Zero;
  GPIO_Init(Switch_Actuator.Ref_Zero.pCheckPort,NULL);
  EXIT_Init(2,3,EXTI2_IRQn,Switch_Actuator.Ref_Zero.pCheckPort);
}


/*******************************************************************************
函 数 名:  void Switch_ONE_Unit_Init(void)
功能说明:  开关单元一初始化
参    数:  无
返 回 值:  无
*******************************************************************************/
void Switch_ONE_Unit_Init(void)
{ 
  Switch_Actuator.Sw_Act_Base[0].pCheckPort         = &Switch_Actuator_Check[0];
  Switch_Actuator.Sw_Act_Base[0].pControlPort       = &Switch_Actuator_Control[0];
  Switch_Actuator.Sw_Act_Base[0].TIM                = TIM2;
  Switch_Actuator.Sw_Act_Base[0].CrossZeroEEPROMAddr= ADDR_CROSS_ZERO_UNIT_ONE_ON;
  Switch_Actuator.Sw_Act_Base[0].PowerOnEEPROMAddr  = SW_POWERON_ONE_ADDR;
  Switch_Actuator.Sw_Act_Base[0].SwActTime.fun      = SW_UNIT_ONE_OFF;
  
  Switch_Actuator.Sw_Act_Base[0].SW_ON              = SW_UNIT_ONE_ON;
  Switch_Actuator.Sw_Act_Base[0].SW_OFF             = SW_UNIT_ONE_OFF;
  Switch_Actuator.Sw_Act_Base[0].SW_REV             = SW_UNIT_ONE_Reverse;
  Switch_Actuator.Sw_Act_Base[0].SW_DelayOpt1       = SW_ONE_DelayOpt1;
  Switch_Actuator.Sw_Act_Base[0].SW_DelayOpt2       = SW_ONE_DelayOpt2;
  Switch_Actuator.Sw_Act_Base[0].SW_DelayClr        = SW_UNIT_ONE_DelayClr;
  
  
  GPIO_Init(Switch_Actuator.Sw_Act_Base[0].pCheckPort,Switch_Actuator.Sw_Act_Base[0].pControlPort);
  EXIT_Init(2,1,EXTI1_IRQn,Switch_Actuator.Sw_Act_Base[0].pCheckPort);
  TIM_Init(1,3,TIM2_IRQn,TIM2);
  SingleSwitchAttr_Init(&Key_OPT[0],&Switch_Actuator.Sw_Act_Base[0]);
}

/*******************************************************************************
函 数 名:  void Switch_TWO_Unit_Init(void)
功能说明:  开关单元一初始化
参    数:  无
返 回 值:  无
*******************************************************************************/
void Switch_TWO_Unit_Init(void)
{  
  Switch_Actuator.Sw_Act_Base[1].pCheckPort           = &Switch_Actuator_Check[1];
  Switch_Actuator.Sw_Act_Base[1].pControlPort         = &Switch_Actuator_Control[1];
  Switch_Actuator.Sw_Act_Base[1].TIM                  = TIM4;
  Switch_Actuator.Sw_Act_Base[1].CrossZeroEEPROMAddr  = ADDR_CROSS_ZERO_UNIT_TWO_ON;
  Switch_Actuator.Sw_Act_Base[1].PowerOnEEPROMAddr    = SW_POWERON_TWO_ADDR;
  Switch_Actuator.Sw_Act_Base[1].SwActTime.fun        = SW_UNIT_TWO_OFF;
  
  Switch_Actuator.Sw_Act_Base[1].SW_ON                = SW_UNIT_TWO_ON;
  Switch_Actuator.Sw_Act_Base[1].SW_OFF               = SW_UNIT_TWO_OFF;
  Switch_Actuator.Sw_Act_Base[1].SW_REV               = SW_UNIT_TWO_Reverse;
  Switch_Actuator.Sw_Act_Base[1].SW_DelayOpt1         = SW_TWO_DelayOpt1;
  Switch_Actuator.Sw_Act_Base[1].SW_DelayOpt2         = SW_TWO_DelayOpt2;
  Switch_Actuator.Sw_Act_Base[1].SW_DelayClr          = SW_UNIT_TWO_DelayClr;
    
  GPIO_Init(Switch_Actuator.Sw_Act_Base[1].pCheckPort,Switch_Actuator.Sw_Act_Base[1].pControlPort);
  EXIT_Init(2,2,EXTI0_IRQn,Switch_Actuator.Sw_Act_Base[1].pCheckPort);
  TIM_Init(1,2,TIM4_IRQn,TIM4);
  SingleSwitchAttr_Init(&Key_OPT[1],&Switch_Actuator.Sw_Act_Base[1]);
}

/*******************************************************************************
函 数 名:  void SW_UNIT_ONE_ON(void)
功能说明:  开关单元一继电器开
参    数:  无
返 回 值:  无
*******************************************************************************/
void SW_UNIT_ONE_ON(void)
{	 
    Switch_ON(&Switch_Actuator.Ref_Zero,&Switch_Actuator.Sw_Act_Base[0],&LED_OPT[0],&Key_OPT[0]);   
}
/*******************************************************************************
函 数 名:  void SW_UNIT_ONE_OFF(void)
功能说明:  开关单元一继电器开
参    数:  无
返 回 值:  无
*******************************************************************************/
void SW_UNIT_ONE_OFF(void)
{	 
    Switch_OFF(&Switch_Actuator.Ref_Zero,&Switch_Actuator.Sw_Act_Base[0],&LED_OPT[0],&Key_OPT[0]); 
}

/*******************************************************************************
函 数 名:  void SW_UNIT_ONE_Reverse(void)
功能说明:  开关单元一继电器状态取反
参    数:  无
返 回 值:  无
*******************************************************************************/
void SW_UNIT_ONE_Reverse(void)
{	 
    Switch_REV(&Switch_Actuator.Ref_Zero,&Switch_Actuator.Sw_Act_Base[0],&LED_OPT[0],&Key_OPT[0]);
}

/*******************************************************************************
函 数 名:  void SW_UNIT_TWO_ON(void)
功能说明:  开关单元二继电器开
参    数:  无
返 回 值:  无
*******************************************************************************/
void SW_UNIT_TWO_ON(void)
{	 
    Switch_ON(&Switch_Actuator.Ref_Zero,&Switch_Actuator.Sw_Act_Base[1],&LED_OPT[1],&Key_OPT[1]);
}

/*******************************************************************************
函 数 名:  void SW_UNIT_TWO_ON(void)
功能说明:  开关单元二继电器开
参    数:  无
返 回 值:  无
*******************************************************************************/
void SW_UNIT_TWO_OFF(void)
{	 
    Switch_OFF(&Switch_Actuator.Ref_Zero,&Switch_Actuator.Sw_Act_Base[1],&LED_OPT[1],&Key_OPT[1]);
}

/*******************************************************************************
函 数 名:  void SW_UNIT_TWO_Reverse(void)
功能说明:  开关单元一继电器状态取反
参    数:  无
返 回 值:  无
*******************************************************************************/
void SW_UNIT_TWO_Reverse(void)
{	 
    Switch_REV(&Switch_Actuator.Ref_Zero,&Switch_Actuator.Sw_Act_Base[1],&LED_OPT[1],&Key_OPT[1]);
}

/*******************************************************************************
函 数 名:  	void SW_UNIT_ONE_DelayClr(void)
功能说明: 	开关延时事件清空
参    数:  	无		
返 回 值:  	无
*******************************************************************************/
void SW_UNIT_ONE_DelayClr(void)
{
	Switch_DelayClr(&Switch_Actuator.Sw_Act_Base[0]);
}

/*******************************************************************************
函 数 名:  	SW_ONE_DelayOpt1
功能说明: 	开关延时操作1
参    数:  	无		
返 回 值:  	无
*******************************************************************************/
void SW_ONE_DelayOpt1(void)
{
  Switch_DelayOpt1(&Switch_Actuator.Sw_Act_Base[0]);
}

/*******************************************************************************
函 数 名:  	SW_ONE_DelayOpt2
功能说明: 	开关延时操作2
参    数:  	无		
返 回 值:  	无
*******************************************************************************/
void SW_ONE_DelayOpt2(void)
{
  Switch_DelayOpt2(&Switch_Actuator.Sw_Act_Base[0]);
}


/*******************************************************************************
函 数 名:  	void SW_UNIT_TWO_DelayClr(void)
功能说明: 	开关延时事件清空
参    数:  	无		
返 回 值:  	无
*******************************************************************************/
void SW_UNIT_TWO_DelayClr(void)
{
	Switch_DelayClr(&Switch_Actuator.Sw_Act_Base[1]);
}

/*******************************************************************************
函 数 名:  	SW_TWO_DelayOpt1
功能说明: 	开关延时操作1
参    数:  	无		
返 回 值:  	无
*******************************************************************************/
void SW_TWO_DelayOpt1(void)
{
  Switch_DelayOpt1(&Switch_Actuator.Sw_Act_Base[1]);
}

/*******************************************************************************
函 数 名:  	SW_TWO_DelayOpt2
功能说明: 	开关延时操作2
参    数:  	无		
返 回 值:  	无
*******************************************************************************/
void SW_TWO_DelayOpt2(void)
{
  Switch_DelayOpt2(&Switch_Actuator.Sw_Act_Base[1]);
}


/*******************************************************************************
函 数 名:  void EXTI2_IRQHandler(void)
功能说明:  过零检测参考点中断
参    数:  无
返 回 值:  无
*******************************************************************************/
void EXTI2_IRQHandler(void)
{	    
  Ref_OPT_Func(&Switch_Actuator.Sw_Act_Base[0]);
  Ref_OPT_Func(&Switch_Actuator.Sw_Act_Base[1]);
  if(!(Switch_Actuator.Sw_Act_Base[0].SW_CrossZero.IRQ_Flag || Switch_Actuator.Sw_Act_Base[1].SW_CrossZero.IRQ_Flag))
  {
      EXTI->IMR&=~((u32)(Switch_Actuator.Ref_Zero.pCheckPort->CheckPin));                 /**> 关参考中断 **/
  }    
  
  EXTI->PR |= ((u32)(Switch_Actuator.Ref_Zero.pCheckPort->CheckPin)); 
}

/*******************************************************************************
函 数 名:  void EXTI0_IRQHandler(void)
功能说明:  开关单元一过零检测验证
参    数:  无
返 回 值:  无
*******************************************************************************/
void EXTI0_IRQHandler(void)
{	 
  Zero_Check_EXTI_Func(&Switch_Actuator.Sw_Act_Base[1]);
  EXTI->IMR &=~(1u<<0);   
  EXTI->PR  |= (1u<<0);     
}

/*******************************************************************************
函 数 名:  void EXTI1_IRQHandler(void)
功能说明:  开关单元二过零检测验证
参    数:  无
返 回 值:  无
*******************************************************************************/
void EXTI1_IRQHandler(void)
{	 
  Zero_Check_EXTI_Func(&Switch_Actuator.Sw_Act_Base[0]);
  EXTI->IMR &=~(1u<<1);   
  EXTI->PR  |= (1u<<1);    
}

/*******************************************************************************
函 数 名:  void TIM2_IRQHandler(void)
功能说明:  开关单元一过零检测验证
参    数:  无
返 回 值:  无
*******************************************************************************/
void TIM2_IRQHandler(void)
{	 
  if(TIM2->SR&0X0001)                                   /**> 判断是否为溢出中断 **/
  {
      Zero_Check_TIM_Func(&Switch_Actuator.Sw_Act_Base[0]); 
  }        
  TIM2->SR&=~(1<<0);
}

/*******************************************************************************
函 数 名:  void TIM4_IRQHandler(void)
功能说明:  开关单元二过零检测验证
参    数:  无
返 回 值:  无
*******************************************************************************/
void TIM4_IRQHandler(void)
{	 
  if(TIM4->SR&0X0001)                                 /**> 判断是否为溢出中断 **/
  {
      Zero_Check_TIM_Func(&Switch_Actuator.Sw_Act_Base[1]); 
  }				   
  TIM4->SR&=~(1<<0);
}

/***********************************公用函数**********************************/
/*******************************************************************************
函 数 名:  static void Switch_ON(pRef_Zero_t pRef_Zero,pSw_Act_Base_t pSw_Act_Base,pLED_OPT_t pLED_OPT,pKey_OPT_t pKey_OPT)
功能说明:  开关单元继电器开
参    数:  pRef_Zero:继电器参考零点指针
           pSw_Act_Base：实际单元执行指针
           pLED_OPT：相应LED指示指针
           pKey_OPT：相应按键指针
返 回 值:  无
*******************************************************************************/
static void Switch_ON(pRef_Zero_t pRef_Zero,pSw_Act_Base_t pSw_Act_Base,pLED_OPT_t pLED_OPT,pKey_OPT_t pKey_OPT)
{	 
  if(0 == (EXTI->IMR&((u32)(pRef_Zero->pCheckPort->CheckPin))))
  {
      EXTI->IMR|= ((u32)(pRef_Zero->pCheckPort->CheckPin));      /**> 开参考中断 **/
  }
  if(0 == pKey_OPT->BindingState)
  {
      if(0 == pKey_OPT->LEDLogicState)
      {
          pLED_OPT->LED_ON();
      }
      else
      {
          pLED_OPT->LED_OFF();
      }
  }
  
  pSw_Act_Base->SW_CrossZero.IRQ_Flag        = 1;
  pSw_Act_Base->SW_CrossZero.RelayActionType = SW_ON;
}
/*******************************************************************************
函 数 名:  static void Switch_OFF(pRef_Zero_t pRef_Zero,pSw_Act_Base_t pSw_Act_Base,pLED_OPT_t pLED_OPT,pKey_OPT_t pKey_OPT)
功能说明:  开关单元一继电器关
参    数:  pRef_Zero:继电器参考零点指针
           pSw_Act_Base：实际单元执行指针
           pLED_OPT：相应LED指示指针
           pKey_OPT：相应按键指针
返 回 值:  无
*******************************************************************************/
static void Switch_OFF(pRef_Zero_t pRef_Zero,pSw_Act_Base_t pSw_Act_Base,pLED_OPT_t pLED_OPT,pKey_OPT_t pKey_OPT)
{	  
  if(0 == (EXTI->IMR&((u32)(pRef_Zero->pCheckPort->CheckPin))))
  {
      EXTI->IMR|= ((u32)(pRef_Zero->pCheckPort->CheckPin));      /**> 开参考中断 **/
  }
  if(0 == pKey_OPT->BindingState)
  {
      if(0 == pKey_OPT->LEDLogicState)
      {
          pLED_OPT->LED_OFF();
      }
      else
      {
          pLED_OPT->LED_ON();
      }
  }
  
  pSw_Act_Base->SW_CrossZero.IRQ_Flag        = 1;
  pSw_Act_Base->SW_CrossZero.RelayActionType = SW_OFF;
}

/*******************************************************************************
函 数 名:  static void Switch_REV(pRef_Zero_t pRef_Zero,pSw_Act_Base_t pSw_Act_Base,pLED_OPT_t pLED_OPT,pKey_OPT_t pKey_OPT)
功能说明:  开关单元继电器状态取反
参    数:  pRef_Zero:继电器参考零点指针
           pSw_Act_Base：实际单元执行指针
           pLED_OPT：相应LED指示指针
           pKey_OPT：相应按键指针
返 回 值:  无
*******************************************************************************/
static void Switch_REV(pRef_Zero_t pRef_Zero,pSw_Act_Base_t pSw_Act_Base,pLED_OPT_t pLED_OPT,pKey_OPT_t pKey_OPT)
{	 
  if(0 == (EXTI->IMR&((u32)(pRef_Zero->pCheckPort->CheckPin))))
  {
      EXTI->IMR|= ((u32)(pRef_Zero->pCheckPort->CheckPin));                 /**> 开参考中断 **/
  }
  if(GPIOx_Get(pSw_Act_Base->pControlPort->ControlPort,
     pSw_Act_Base->pControlPort->ControlPin))                               /**> 若当前状态为开，取反后为关 **/
  {
      pSw_Act_Base->SW_CrossZero.RelayActionType = SW_OFF; 
      
      if(0 == pKey_OPT->BindingState)
      {
          if(0 == pKey_OPT->LEDLogicState)
          {
              pLED_OPT->LED_OFF();
          }
          else
          {
              pLED_OPT->LED_ON();
          }
      }       
  }
  else
  {
      pSw_Act_Base->SW_CrossZero.RelayActionType = SW_ON;

      if(0 == pKey_OPT->BindingState)
      {
          if(0 == pKey_OPT->LEDLogicState)
          {
              pLED_OPT->LED_ON();
          }
          else
          {
              pLED_OPT->LED_OFF();
          }
      }        
  }
  
  pSw_Act_Base->SW_CrossZero.IRQ_Flag = 1;   
}

/*******************************************************************************
函 数 名:  static void Ref_OPT_Func(pSw_Act_Base_t pSw_Act_Base)
功能说明:  参考零点实际执行函数
参    数:  pSw_Act_Base：实际单元执行指针
返 回 值:  无
*******************************************************************************/
static void Ref_OPT_Func(pSw_Act_Base_t pSw_Act_Base)
{
    if(1 == pSw_Act_Base->SW_CrossZero.IRQ_Flag)
    {
        pSw_Act_Base->SW_CrossZero.IRQ_Flag = 0;    
        if(1 == pSw_Act_Base->SW_CrossZero.CheckFlagON)
        {
            if(SW_ON == pSw_Act_Base->SW_CrossZero.RelayActionType) 
            {
                pSw_Act_Base->SW_CrossZero.CheckFlagON = 0;  
                EXTI->FTSR |= ((u32)(pSw_Act_Base->pCheckPort->CheckPin));          /**> 继电器闭合检测下降沿触发 **/
                EXTI->RTSR &=~((u32)(pSw_Act_Base->pCheckPort->CheckPin));          /**> 继电器闭合检测下降沿触发 **/
                EXTI->IMR  |= ((u32)(pSw_Act_Base->pCheckPort->CheckPin));          /**<  开启line上的中断 */
                pSw_Act_Base->TIM->SR &=~(1<<0);
                pSw_Act_Base->TIM->CNT = 0;
                pSw_Act_Base->TIM->CR1|= (1<<0);                                    /**< 使能定时器 */
                pSw_Act_Base->TIM->DIER&=~(1<<0);                                   /**< 不允许更新中断 */
                
                GPIOx_Set(pSw_Act_Base->pControlPort->ControlPort,                  /**> 开继电器 **/        
                          pSw_Act_Base->pControlPort->ControlPin);
            }
            else
            {
                pSw_Act_Base->TIM->ARR = (u16)((u16)(AC_FRE_VALUE/2) - RELAY_CROSSZERO_OFF_TIME);//6200;  
                EXTI->IMR&=~((u32)(pSw_Act_Base->pCheckPort->CheckPin));            /**<  关闭line上的中断 */ 
                pSw_Act_Base->TIM->CNT = 0;
                pSw_Act_Base->TIM->SR &=~(1<<0);
                pSw_Act_Base->TIM->CR1|= (1<<0);                                    /**< 使能定时器 */
                pSw_Act_Base->TIM->DIER|=(1<<0);                                    /**< 允许更新中断 */
            }    
        }
        else if(1 == pSw_Act_Base->SW_CrossZero.CheckFlagOFF)
        {
            if(SW_OFF == pSw_Act_Base->SW_CrossZero.RelayActionType) 
            {
                pSw_Act_Base->SW_CrossZero.CheckFlagOFF = 0; 
                EXTI->RTSR |= ((u32)(pSw_Act_Base->pCheckPort->CheckPin));          /**> 继电器闭合检测上升沿触发 **/
                EXTI->FTSR &=~((u32)(pSw_Act_Base->pCheckPort->CheckPin));
                EXTI->IMR  |= ((u32)(pSw_Act_Base->pCheckPort->CheckPin));          /**<  开启line上的中断 */
                
                pSw_Act_Base->TIM->SR &=~(1<<0);
                pSw_Act_Base->TIM->CNT = 0;
                pSw_Act_Base->TIM->CR1|= (1<<0);                                    /**< 使能定时器 */
                pSw_Act_Base->TIM->DIER&=~(1<<0);                                   /**< 不允许更新中断 */
                
                GPIOx_Rst(pSw_Act_Base->pControlPort->ControlPort,                  /**> 关继电器 **/        
                          pSw_Act_Base->pControlPort->ControlPin);
            } 
            else
            {
                pSw_Act_Base->TIM->ARR = (u16)((u16)(AC_FRE_VALUE/2) - RELAY_CROSSZERO_ON_TIME);//3050; 
                EXTI->IMR &=~((u32)(pSw_Act_Base->pCheckPort->CheckPin));           /**<  关闭line上的中断 */ 
                pSw_Act_Base->TIM->CNT = 0;
                pSw_Act_Base->TIM->SR &=~(1<<0);
                pSw_Act_Base->TIM->CR1|= (1<<0);                                    /**< 使能定时器 */
                pSw_Act_Base->TIM->DIER|=(1<<0);                                    /**< 允许更新中断 */
            }    
        }
        else
        {
            switch(pSw_Act_Base->SW_CrossZero.RelayActionType)
            {
                case SW_ON :
                {
                    if((pSw_Act_Base->SW_CrossZero.CrossZeroTimeON < RELAY_CROSSZERO_ON_LOWER_LIMIT)||    /**> 过零时间检查，防止过零时间丢失，过零保护失败 **/
                        (pSw_Act_Base->SW_CrossZero.CrossZeroTimeON > RELAY_CROSSZERO_ON_UPPER_LIMIT))  
                    {
                        pSw_Act_Base->SW_CrossZero.CheckFlagON = 1;
                        pSw_Act_Base->TIM->ARR = (u16)((u16)(AC_FRE_VALUE/2) - RELAY_CROSSZERO_ON_TIME);//3050; 
                    } 
                    else  
                    {                        
                        pSw_Act_Base->TIM->ARR=(u16)(AC_FRE_VALUE/2)-pSw_Act_Base->SW_CrossZero.CrossZeroTimeON;
                    }
                }
                break;
                case SW_OFF :
                {
                    if((pSw_Act_Base->SW_CrossZero.CrossZeroTimeOFF < RELAY_CROSSZERO_OFF_LOWER_LIMIT)||  /**> 过零时间检查，防止过零时间丢失，过零保护失败 **/
                       (pSw_Act_Base->SW_CrossZero.CrossZeroTimeOFF > RELAY_CROSSZERO_OFF_UPPER_LIMIT))
                    {
                        pSw_Act_Base->SW_CrossZero.CheckFlagOFF = 1;
                        pSw_Act_Base->TIM->ARR = (u16)((u16)(AC_FRE_VALUE/2) - RELAY_CROSSZERO_OFF_TIME);//6200; 
                    } 
                    else
                    {
                        pSw_Act_Base->TIM->ARR=(u16)(AC_FRE_VALUE/2)-pSw_Act_Base->SW_CrossZero.CrossZeroTimeOFF;
                    }
                }
                break;                
                default:break;
            }
            EXTI->IMR&=~((u32)(pSw_Act_Base->pCheckPort->CheckPin));                    /**<  关闭line上的中断 */ 
            pSw_Act_Base->TIM->CNT = 0;
            pSw_Act_Base->TIM->SR &=~(1<<0);
            pSw_Act_Base->TIM->CR1|= (1u<<0);
            pSw_Act_Base->TIM->DIER|=(1<<0);                                           /**< 允许更新中断 */
        }
   }
}

/*******************************************************************************
函 数 名:  static void Zero_Check_EXTI_Func(pSw_Act_Base_t pSw_Act_Base)
功能说明:  外部中断实际执行函数
参    数:  pSw_Act_Base：实际单元执行指针
返 回 值:  无
*******************************************************************************/
static void Zero_Check_EXTI_Func(pSw_Act_Base_t pSw_Act_Base)
{
    switch(pSw_Act_Base->SW_CrossZero.RelayActionType)
    {
        case SW_ON :
        {
            pSw_Act_Base->SW_CrossZero.CrossZeroTimeON = pSw_Act_Base->TIM->CNT;
            pSw_Act_Base->TIM->CR1 &=~(1u<<0);
            if((pSw_Act_Base->SW_CrossZero.CrossZeroTimeON < RELAY_CROSSZERO_ON_LOWER_LIMIT)||
                (pSw_Act_Base->SW_CrossZero.CrossZeroTimeON > RELAY_CROSSZERO_ON_UPPER_LIMIT))  
            {
                pSw_Act_Base->SW_CrossZero.CrossZeroTimeON = RELAY_CROSSZERO_ON_TIME;   
            }                
            WriteDataToEEPROM(pSw_Act_Base->CrossZeroEEPROMAddr,\
                              sizeof(pSw_Act_Base->SW_CrossZero.CrossZeroTimeON),\
                              (u8 *)(&pSw_Act_Base->SW_CrossZero.CrossZeroTimeON));
            /***************************解决首次定时不准问题**********************/
            pSw_Act_Base->TIM->ARR = 2;
            pSw_Act_Base->TIM->CR1|= (1u<<0);                               /**> 开定时器 **/
            pSw_Act_Base->TIM->SR &=~(1<<0);
            pSw_Act_Base->TIM->DIER|=(1<<0);                                /**< 允许更新中断 */                  
            pSw_Act_Base->TIM_First_InitFlag = 1;                  
        }
        break;
        case SW_OFF :
        {
            pSw_Act_Base->SW_CrossZero.CrossZeroTimeOFF = pSw_Act_Base->TIM->CNT;
            pSw_Act_Base->TIM->CR1 &= ~(1u<<0);
            if((pSw_Act_Base->SW_CrossZero.CrossZeroTimeOFF < RELAY_CROSSZERO_OFF_LOWER_LIMIT)||
               (pSw_Act_Base->SW_CrossZero.CrossZeroTimeOFF > RELAY_CROSSZERO_OFF_UPPER_LIMIT))
            {
                pSw_Act_Base->SW_CrossZero.CrossZeroTimeOFF = RELAY_CROSSZERO_OFF_TIME;   
            }            
            WriteDataToEEPROM(pSw_Act_Base->CrossZeroEEPROMAddr + offsetof(SW_CrossZero_t, CrossZeroTimeOFF),\
                              sizeof(pSw_Act_Base->SW_CrossZero.CrossZeroTimeOFF),\
                              (u8 *)(&pSw_Act_Base->SW_CrossZero.CrossZeroTimeOFF));                   
            /***************************解决首次定时不准问题**********************/
            pSw_Act_Base->TIM->ARR = 2;
            pSw_Act_Base->TIM->CR1|= (1u<<0);                               /**> 开定时器 **/
            pSw_Act_Base->TIM->SR &=~(1<<0);
            pSw_Act_Base->TIM->DIER|=(1<<0);                                /**< 允许更新中断 */                  
            pSw_Act_Base->TIM_First_InitFlag = 1; 
        }
        break;        
        default:break;
    }
}

/*******************************************************************************
函 数 名:  static void Zero_Check_TIM_Func(pSw_Act_Base_t pSw_Act_Base)
功能说明:  定时器中断实际执行函数
参    数:  pSw_Act_Base：实际单元执行指针
返 回 值:  无
*******************************************************************************/
static void Zero_Check_TIM_Func(pSw_Act_Base_t pSw_Act_Base)
{
    if(1 == pSw_Act_Base->TIM_First_InitFlag)                              /**> 解决首次使用定时器定时不准的问题 **/
    {
        pSw_Act_Base->TIM_First_InitFlag = 0;
        pSw_Act_Base->TIM->CR1 &=~(1u<<0);
        pSw_Act_Base->TIM->DIER&=~(1u<<0);                                 /**< 禁止更新中断 */
    }
    else
    {
        switch(pSw_Act_Base->SW_CrossZero.RelayActionType)
        {
            case SW_ON :
            {
                GPIOx_Set(pSw_Act_Base->pControlPort->ControlPort,pSw_Act_Base->pControlPort->ControlPin);
            }
            break;
            case SW_OFF :
            {
                GPIOx_Rst(pSw_Act_Base->pControlPort->ControlPort,pSw_Act_Base->pControlPort->ControlPin);
            }
            break;
            default :break;
        }
        pSw_Act_Base->TIM->CR1 &=~(1u<<0);
        pSw_Act_Base->TIM->DIER&=~(1u<<0);                                 /**< 禁止更新中断 */
    }  
}

/*******************************************************************************
函 数 名:  	static void Switch_DelayOpt1(pSw_Act_Base_t pSw_Act_Base)
功能说明: 	开关延时操作1
参    数:  	pSw_Act_Base：实际单元执行指针		
返 回 值:  	无
*******************************************************************************/
static void Switch_DelayOpt1(pSw_Act_Base_t pSw_Act_Base)
{
  pSw_Act_Base->SwActTime.fun();
  if (0 != pSw_Act_Base->SwActTime.secondTime)	                          /**> 保持一段时间后关闭 **/
	{
		Thread_Login(ONCEDELAY, 0, pSw_Act_Base->SwActTime.secondTime, pSw_Act_Base->SW_DelayOpt2);
	} 
}

/*******************************************************************************
函 数 名:  	static void Switch_DelayOpt2(pSw_Act_Base_t pSw_Act_Base)
功能说明: 	开关延时操作2
参    数:  	pSw_Act_Base：实际单元执行指针		
返 回 值:  	无
*******************************************************************************/
static void Switch_DelayOpt2(pSw_Act_Base_t pSw_Act_Base)
{
  pSw_Act_Base->SW_REV();                           /**> 取反当前状态 **/
	if (0 != pSw_Act_Base->SwActTime.cycleFlag)		    /**> 操作循环 **/
	{
		Thread_Login(ONCEDELAY, 0, pSw_Act_Base->SwActTime.firstTime,pSw_Act_Base->SW_DelayOpt1);
	}
}

/*******************************************************************************
函 数 名:  	static void Switch_DelayClr(pSw_Act_Base_t pSw_Act_Base)
功能说明: 	开关延时事件清空
参    数:  	pSw_Act_Base：实际单元执行指针		
返 回 值:  	无
*******************************************************************************/
static void Switch_DelayClr(pSw_Act_Base_t pSw_Act_Base)
{
  	Thread_Logout(pSw_Act_Base->SwActTime.fun);
    Thread_Logout(pSw_Act_Base->SW_DelayOpt1);
    Thread_Logout(pSw_Act_Base->SW_DelayOpt2);		                        /**> 清空所有开关延时操作线程 **/
	
    pSw_Act_Base->SwActTime.fun        = pSw_Act_Base->SW_OFF;	
    pSw_Act_Base->SwActTime.cycleFlag  = 0;
    pSw_Act_Base->SwActTime.firstTime  = 0;
    pSw_Act_Base->SwActTime.secondTime = 0;
    
    pSw_Act_Base->SW_CrossZero.IRQ_Flag = 0;
    pSw_Act_Base->TIM->DIER            &=~(1<<0);                          /**< 禁止更新中断  **/

    pSw_Act_Base->TIM->SR              &=~(1u<<0); 
    pSw_Act_Base->TIM->CR1             &=~(1u<<0);
    pSw_Act_Base->TIM->CNT              = 0;
    
    EXTI->IMR &=~((u32)(pSw_Act_Base->pCheckPort->CheckPin));              /**<  关闭line上的中断 */
}

/*******************************************************************************
函 数 名:  static void SingleSwitchAttr_Init(pKey_OPT_t pKey_OPT,pSw_Act_Base_t pSw_Act_Base)
功能说明:  执行器单元的初始化
参    数:  pKey_OPT:按键资源结构体指针
           pSw_Act_Base:开关执行器结构体指针
返 回 值:  无
*******************************************************************************/
static void SingleSwitchAttr_Init(pKey_OPT_t pKey_OPT,pSw_Act_Base_t pSw_Act_Base)
{
    pSw_Act_Base->SW_CrossZero.CheckFlagON = 0;
    pSw_Act_Base->SW_CrossZero.CheckFlagOFF= 0;
       
    ReadDataFromEEPROM(pSw_Act_Base->CrossZeroEEPROMAddr,sizeof(pSw_Act_Base->SW_CrossZero.CrossZeroTimeON),(u8 *)(&pSw_Act_Base->SW_CrossZero.CrossZeroTimeON));
    ReadDataFromEEPROM(pSw_Act_Base->CrossZeroEEPROMAddr + sizeof(pSw_Act_Base->SW_CrossZero.CrossZeroTimeON),sizeof(pSw_Act_Base->SW_CrossZero.CrossZeroTimeOFF),(u8 *)(&pSw_Act_Base->SW_CrossZero.CrossZeroTimeOFF));
    
    if((pSw_Act_Base->SW_CrossZero.CrossZeroTimeON < RELAY_CROSSZERO_ON_LOWER_LIMIT)||
       (pSw_Act_Base->SW_CrossZero.CrossZeroTimeON > RELAY_CROSSZERO_ON_UPPER_LIMIT))
    {
        pSw_Act_Base->SW_CrossZero.CheckFlagON = 1;
    }
    
    if((pSw_Act_Base->SW_CrossZero.CrossZeroTimeOFF < RELAY_CROSSZERO_OFF_LOWER_LIMIT)||
       (pSw_Act_Base->SW_CrossZero.CrossZeroTimeOFF > RELAY_CROSSZERO_OFF_UPPER_LIMIT))
    {
        pSw_Act_Base->SW_CrossZero.CheckFlagOFF = 1;
    }
    
    if(!(pSw_Act_Base->SW_CrossZero.CheckFlagOFF||pSw_Act_Base->SW_CrossZero.CheckFlagON))           /**> 解决首次使用定时器定时不准的问题 **/
    {
        pSw_Act_Base->TIM->ARR           = 2;
        pSw_Act_Base->TIM->CR1          |= (1u<<0);
        pSw_Act_Base->TIM->SR           &=~(1<<0);
        pSw_Act_Base->TIM->DIER         |= (1u<<0);
        pSw_Act_Base->TIM_First_InitFlag = 1;   
    }
    Thread_Login(ONCEDELAY, 0, 500, pSw_Act_Base->SW_ON);
    EEPROM_Read(pSw_Act_Base->PowerOnEEPROMAddr, sizeof(pSw_Act_Base->PowerOnState), (u8*)&pSw_Act_Base->PowerOnState);	
    EEPROM_Read(pKey_OPT->BindingEEPROMAddr, sizeof(pKey_OPT->BindingState) + sizeof(pKey_OPT->LEDLogicState), (u8*)&pKey_OPT->BindingState);	
   
    if (0 == pSw_Act_Base->PowerOnState)		                                                    /** 上电状态为零 **/
    {	
      Thread_Login(ONCEDELAY, 0, 5500, pSw_Act_Base->SW_OFF);
    }
    if(pKey_OPT->BindingState>1)
    {
        pKey_OPT->BindingState = 0x00;
    }

    if(pKey_OPT->LEDLogicState>1)
    {
        pKey_OPT->LEDLogicState = 0x00;
    }

    if((pKey_OPT->BindingState>1)||
       (pKey_OPT->LEDLogicState>1))
    {
        EEPROM_Write(pKey_OPT->BindingEEPROMAddr, sizeof(pKey_OPT->BindingState) + sizeof(pKey_OPT->LEDLogicState), (u8*)&pKey_OPT->BindingState);
    }
}

/*********************************End if File********************************************/
