/***************************Copyright BestFu 2015-11-02*************************
文 件:	KeyUnit.c
说 明:	开关执行器驱动性实现文件
编 辑:	Keil uVision5 V5.12
版 本:	v1.0
编 写:	Jay
日 期:	2015.11.02
修 改:	暂无
*******************************************************************************/
#include "KeyUnit.h"
#include "sys.h"
#include "relay.h"
#include "SwitchUnit.h"
#include "LedUnit.h"
#include "Thread.h"

/*******************************变量定义*************************************/
const KeyPort_t KeyPort[3]  = {{PORT_GPIO_KEY_1,PIN_GPIO_KEY_1},
							   {PORT_GPIO_KEY_2,PIN_GPIO_KEY_2},
							   {PORT_GPIO_KEY_3,PIN_GPIO_KEY_3}};
const u16 KEY_EEPROM_ADDR[3]= {SW_BINDING_ONE_ADDR,SW_BINDING_TWO_ADDR,0};

Key_OPT_t Key_OPT[3];

/*******************************函数声明*************************************/
static void KeyBindingRelayHandle(u8 unit,u8 StateCMD,u8 ActionCMD,pKey_OPT_t pKey_OPT,pSw_Act_Base_t pSw_Act_Base);
static void KeyNoBindingRelayHandle(u8 unit,u8 StateCMD,u8 ActionCMD,pKey_OPT_t pKey_OPT);
static u8 InquireKey(void);
/*******************************************************************************
函 数 名:  void Key_Unit_Init(void)
功能说明:  按键单元初始化
参    数:  无
返 回 值:  无
*******************************************************************************/
void Key_Unit_Init(void)
{
	u8 i = 0,ucCNT = sizeof(Key_OPT)/sizeof(Key_OPT[0]);
/*********************************按键初始化*********************************/
	for(i = 0;i < ucCNT;i++)
	{
		Key_OPT[i].KEY.pKeyPort       = (KeyPort_t *)&KeyPort[i];
		Key_OPT[i].KeyAct.Status      = (KEY_TYPE_t)(0x00);
		Key_OPT[i].KeySta.Status      = (KEY_TYPE_t)(0x00);
		Key_OPT[i].BindingEEPROMAddr  = KEY_EEPROM_ADDR[i];   
		WTC6104BSI_Init(&Key_OPT[i]);
	}	
}


/*******************************************************************************
函 数 名:  static void KeyBindingRelayHandle(RelayAction_t *pRelayActionWithKey)
功能说明:  检测触摸按键是否被按下
参    数:  unit:按键所在的单元号
           StateCMD:状态资源按键属性号
           ActionCMD:动作资源按键属性号
           pKey_OPT:按键资源结构体指针
           pSw_Act_Base:开关执行器结构体指针
返 回 值:  无
*******************************************************************************/
static void KeyBindingRelayHandle(u8 unit,u8 StateCMD,u8 ActionCMD,pKey_OPT_t pKey_OPT,pSw_Act_Base_t pSw_Act_Base)
{  
    switch(pKey_OPT->KEY.Key_Type)
    {
        case KEY_SHORT_PRESS:
        {   
            if(GPIOx_Get(pSw_Act_Base->pControlPort->ControlPort,      /**> 若当前状态为开，取反后为关 **/
               pSw_Act_Base->pControlPort->ControlPin))  
            {
                PropEventFifo(unit,StateCMD, SRCEVENT , SWITCH_OFF);        
            }
            else
            {
                PropEventFifo(unit,StateCMD, SRCEVENT , SWITCH_ON);        
            }
            pKey_OPT->KeyAct.Status   = (KEY_TYPE_t)(0x01);
            PropEventFifo(unit,ActionCMD, SRCEVENT , KEY_SHORT);
            pSw_Act_Base->SW_DelayClr();
            pSw_Act_Base->SW_REV();  
        }
        break;
        case KEY_LONG_PRESS:
        {
            pKey_OPT->KeyAct.Status = (KEY_TYPE_t)(0x02);
            PropEventFifo(unit,ActionCMD, SRCEVENT , KEY_LONG); 
        }
        break;
        default:break;
    }
        
}

/*******************************************************************************
函 数 名:  static void KeyNoBindingRelayHandle(u8 unit,u8 StateCMD,u8 ActionCMD,pKey_OPT_t pKey_OPT)
功能说明:  检测触摸按键是否被按下
参    数:  unit:按键所在的单元号
           StateCMD:状态资源按键属性号
           ActionCMD:动作资源按键属性号
           pKey_OPT:按键资源结构体指针
返 回 值:  无
*******************************************************************************/
static void KeyNoBindingRelayHandle(u8 unit,u8 StateCMD,u8 ActionCMD,pKey_OPT_t pKey_OPT)
{
    switch(pKey_OPT->KEY.Key_Type)
    {
        case KEY_SHORT_PRESS:
        {  
            if(pKey_OPT->KeySta.Status)
            {
                pKey_OPT->KeySta.Status = (KEY_TYPE_t)(0x00);
                PropEventFifo(unit,StateCMD, SRCEVENT , SWITCH_OFF); 
            }
            else
            {
                pKey_OPT->KeySta.Status = (KEY_TYPE_t)(0x01);
                PropEventFifo(unit,StateCMD, SRCEVENT , SWITCH_ON); 
            }
            pKey_OPT->KeyAct.Status = KEY_SHORT_PRESS;
            PropEventFifo(unit,ActionCMD, SRCEVENT , KEY_SHORT);    
        }
        break;
        case KEY_LONG_PRESS:
        {
            pKey_OPT->KeyAct.Status = KEY_LONG_PRESS;
            PropEventFifo(unit,ActionCMD, SRCEVENT , KEY_LONG); 
        }
        break;
        default:break;
    }
}

/*******************************************************************************
函 数 名:  static u8 InquireKey(void)
功能说明:  查询被按下的键值
参    数:  无
返 回 值:  有效的键号，0x00表示没有键值
*******************************************************************************/
static u8 InquireKey(void)
{
	u8 ucKeyNum = 0x00;
	u8 i;
	for(i = 0;i < sizeof(Key_OPT)/sizeof(Key_OPT[0]);i++)
	{
		if(KEY_NONE_PRESS != Key_OPT[i].KEY.Key_Type)
		{
			ucKeyNum |= (1U << i);
		}
	}
	return (ucKeyNum);
}

/*******************************************************************************
函 数 名:  void TouchSwitch(void)
功能说明:  检测触摸按键是否被按下
参    数:  无
返 回 值:  无
*******************************************************************************/
void Key_Handle(void)
{
    u8 keynum = 0x00;
	
	Key_RefreshState(&Key_OPT[0].KEY);
	Key_RefreshState(&Key_OPT[1].KEY);
	Key_RefreshState(&Key_OPT[2].KEY);
    
	keynum = InquireKey();
	if(0x00 != keynum)
	{
		Key_OPT[0].KeyAct.Status = KEY_NONE_PRESS;
		Key_OPT[1].KeyAct.Status = KEY_NONE_PRESS;
		Key_OPT[2].KeyAct.Status = KEY_NONE_PRESS;
		
		if((0 == Key_OPT[0].BindingState)&&
		   (0 == Key_OPT[1].BindingState))
		{
			KeyBindingRelayHandle(KEY_UNIT,KEY1_STATE_CMD,KEY1_ACTION_CMD,&Key_OPT[0],&Switch_Actuator.Sw_Act_Base[0]);
			KeyBindingRelayHandle(KEY_UNIT,KEY2_STATE_CMD,KEY2_ACTION_CMD,&Key_OPT[1],&Switch_Actuator.Sw_Act_Base[1]);
			if(0x04 == keynum)
			{
				LED_Flash_Func(LED_3_ON,3,LED_NULL);
				KeyNoBindingRelayHandle(KEY_UNIT,KEY3_STATE_CMD,KEY3_ACTION_CMD,&Key_OPT[2]);
			}
		}
		else if(0 == Key_OPT[0].BindingState)
		{
			KeyBindingRelayHandle(KEY_UNIT,KEY1_STATE_CMD,KEY1_ACTION_CMD,&Key_OPT[0],&Switch_Actuator.Sw_Act_Base[0]);
			switch(keynum)
			{
				case 0x02:
				{
					LED_Flash_Func(LED_23_ON,2,LED_3_OFF);
					KeyNoBindingRelayHandle(KEY_UNIT,KEY2_STATE_CMD,KEY2_ACTION_CMD,&Key_OPT[1]);
				}break;
				case 0x04:
				{
					LED_Flash_Func(LED_23_ON,3,LED_2_OFF);
					KeyNoBindingRelayHandle(KEY_UNIT,KEY3_STATE_CMD,KEY3_ACTION_CMD,&Key_OPT[2]);
				}break;
				default:break;
			}
		}
		else if(0 == Key_OPT[1].BindingState)
		{
			KeyBindingRelayHandle(KEY_UNIT,KEY2_STATE_CMD,KEY2_ACTION_CMD,&Key_OPT[1],&Switch_Actuator.Sw_Act_Base[1]);
			switch(keynum)
			{
				case 0x01:
				{
					LED_Flash_Func(LED_13_ON,1,LED_3_OFF);
					KeyNoBindingRelayHandle(KEY_UNIT,KEY1_STATE_CMD,KEY1_ACTION_CMD,&Key_OPT[0]);
				}break;
				case 0x04:
				{
					LED_Flash_Func(LED_13_ON,3,LED_1_OFF);
					KeyNoBindingRelayHandle(KEY_UNIT,KEY3_STATE_CMD,KEY3_ACTION_CMD,&Key_OPT[2]);
				}break;
				default:break;
			}
		}
		else
		{
			switch(keynum)
			{
				case 0x01:
				{
					LED_Flash_Func(LED_ALL_ON,1,LED_23_OFF);
					KeyNoBindingRelayHandle(KEY_UNIT,KEY1_STATE_CMD,KEY1_ACTION_CMD,&Key_OPT[0]);
				}break;
				case 0x02:
				{
					LED_Flash_Func(LED_ALL_ON,2,LED_13_OFF);
					KeyNoBindingRelayHandle(KEY_UNIT,KEY2_STATE_CMD,KEY2_ACTION_CMD,&Key_OPT[1]);
				}break;
				case 0x04:
				{
					LED_Flash_Func(LED_ALL_ON,3,LED_12_OFF);
					KeyNoBindingRelayHandle(KEY_UNIT,KEY3_STATE_CMD,KEY3_ACTION_CMD,&Key_OPT[2]);
				}break;
				default:break;
			}
		}
        Upload(); 
	}
}

/******************************************************************************/

