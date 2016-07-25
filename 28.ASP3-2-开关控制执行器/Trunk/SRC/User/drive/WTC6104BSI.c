/***************************Copyright BestFu 2015-08-03*************************
文 件:	WTC6104BSI.c
说 明:	开关执行器设备驱动实现文件
编 辑:	MDK-ARM Professional Version: 5.12.0.0
版 本:	v1.0
编 写:	Jay
日 期:	2015.11.02
修 改:	暂无
*******************************************************************************/
#include "WTC6104BSI.h"

static void KeyCreate(KEY_t *p_Key);
static u8 GetPinStateOfKey(pKeyPort_t pKeyPort);
/*******************************************************************************
**函    数:  void WTC6104BSI_Init(pKey_OPT_t pKey_OPT,KEY_CALLBACK_PTR pKey_Callback)
**功    能:  WTC6104BSI初始化
**参    数:  pKey_OPT:按键初始化结构体
             pKey_Callback：获取按键状态的回调函数指针
**返    回:  null
*******************************************************************************/
void WTC6104BSI_Init(pKey_OPT_t pKey_OPT)
{
    GPIOx_Cfg(pKey_OPT->KEY.pKeyPort->CheckPort,pKey_OPT->KEY.pKeyPort->CheckPin,IN_UPDOWN);  /**> 输入模式          **/
    GPIOx_Set(pKey_OPT->KEY.pKeyPort->CheckPort,pKey_OPT->KEY.pKeyPort->CheckPin);
    
    KeyCreate(&pKey_OPT->KEY);                                          /**> 创建按键KEY       **/
}

/*******************************************************************************
**函    数： static void KeyCreate(KEY_t *p_Key,KEY_CALLBACK_PTR p_CallBack)
**功    能： 创建按键
**参    数： p_Key：创建按键的指针
             p_CallBack:按键的回调函数
**返    回： void
*******************************************************************************/
static void KeyCreate(KEY_t *p_Key)
{
  p_Key->GetStatePtr=GetPinStateOfKey;
  p_Key->State=KEY_UP;
  p_Key->Time_ms=0;        
  p_Key->Key_Type = KEY_NONE_PRESS;
  p_Key->Key_Count=0;
}

/*******************************************************************************
**函    数:  static u8 GetPinStateOfKey(void)
**功    能:  WTC6104BSI读取按键KEY1的值
**参    数:  pKeyPort:待获取的GPIO pin指针
**返    回:  1：表示有按键，0：表示无按键
*******************************************************************************/
static u8 GetPinStateOfKey(pKeyPort_t pKeyPort)
{
    return (GPIOx_Get(pKeyPort->CheckPort,pKeyPort->CheckPin))?(0):(1);
}

/*******************************************************************************
**函    数： void Key_RefreshState(KEY_t* p_Key)
**功    能： 状态机方式扫描按键
**参    数： *keyFlag    按键标志    0-无按键  1-短按键    2-长按键 
**返    回： 按键值
**说    明： 该函数 每隔 5~20ms 调用一次
*******************************************************************************/
void Key_RefreshState(KEY_t* p_Key)
{
  switch(p_Key->State)
  {
    case KEY_UP:
    {
      if((*(p_Key->GetStatePtr))(p_Key->pKeyPort))               /**> 判断是否有键按下 **/
      {  
        p_Key->State = KEY_DOWN_WOBBLE;                          
      }
      else
      {
        p_Key->State = KEY_UP; 
      }
      p_Key->Time_ms = 0;
      p_Key->Key_Type = KEY_NONE_PRESS;
    }
    break;
    case KEY_DOWN_WOBBLE:                                        /**> 按下消抖处理  **/
    {
      p_Key->Time_ms += KEY_FIXED_PERIOD;
      if( p_Key->Time_ms >=KEY_WOBBLE_TIME )
      {
        if((*(p_Key->GetStatePtr))(p_Key->pKeyPort))
        {
          p_Key->State = KEY_DOWN;
        }
        else
        {
          p_Key->State = KEY_UP;
        }
        p_Key->Key_Count = 0;
        p_Key->Time_ms = 0;
      }
    }
    break;
    case KEY_DOWN:                                              /**> 按键确认按下 **/
    {
      if( (*(p_Key->GetStatePtr))(p_Key->pKeyPort))                            
      {
        if(p_Key->Key_Count++ == KEY_LONG_CNT)
        {
           p_Key->Key_Type = KEY_LONG_PRESS;
        } 
        else
        {
            p_Key->Key_Type = KEY_NONE_PRESS;
        }    
      }
      else
      {
        p_Key->State = KEY_UP_WOBBLE; 
      }
    }
    break;
    case KEY_UP_WOBBLE:                                         /**> 弹起消抖处理 **/
    {
      p_Key->Time_ms += KEY_FIXED_PERIOD;
      if( p_Key->Time_ms >= KEY_WOBBLE_TIME )
      {
        if( (*(p_Key->GetStatePtr))(p_Key->pKeyPort))
        {
            p_Key->State = KEY_DOWN;
        }
        else
        {
            p_Key->State = KEY_UP;
            if(p_Key->Key_Count < KEY_LONG_CNT)
            {
              p_Key->Key_Type = KEY_SHORT_PRESS;
            }
            p_Key->Key_Count = 0;
         }
         p_Key->Time_ms = 0;
      }
    }
    break;
    default:
    {
        p_Key->State = KEY_UP;
        p_Key->Time_ms = 0;
        p_Key->Key_Type = KEY_NONE_PRESS;
        p_Key->Key_Count = 0;
    }
    break;
  }
}
/****************************************End of File*******************************************/
