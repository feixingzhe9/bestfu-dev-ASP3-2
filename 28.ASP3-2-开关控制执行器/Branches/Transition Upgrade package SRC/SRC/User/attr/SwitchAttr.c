/***************************Copyright BestFu 2015-08-03*************************
文 件:	SwitchAttr.c
说 明:	开关执行器设备属性实现文件
编 辑:	Keil uVision5 V5.12
版 本:	v1.0
编 写:	Jay
日 期:	2015.08.03
修 改:	暂无
*******************************************************************************/
#include "SwitchAttr.h"
#include "SwitchUnit.h"
#include "relay.h"
#include "WTC6104BSI.h"
#include "Thread.h"
#include "EEPROM.h"
#include "LedUnit.h"
#include "stm32f10x.h"
#include "keyUnit.h"


//对属性读写的接口列表
const AttrExe_st SwitchAttrTable[] =
{
    {0x01, LEVEL_1, Set_SwActSwitch_Attr        ,Get_SwActSwitch_Attr        },
    {0x02, LEVEL_0, Set_SWActionTimer_Attr      ,NULL                        },
    {0x08, LEVEL_0, Set_SWPowerOnStatus_Attr    ,Get_SWPowerOnStatus_Attr    },
    {0x0a, LEVEL_0, Set_SW_ZeroCross_Attr_Enable,Get_SW_ZeroCross_Attr_Enable},     /**> 测试接口 **/
    {0x0b, LEVEL_0, NULL                        ,Get_SW_ZeroCross_Attr       },     /**> 测试接口 **/
    {0x80, LEVEL_0, Set_SWActReversal_Attr      ,NULL                        },
};

/*******************************************************************************
函 数 名:  void SwitchAttr_Init(u8 unitID)
功能说明:  执行器单元的初始化
参    数:  unitID-单元号
返 回 值:  无
*******************************************************************************/
void SwitchAttr_Init(u8 unitID)
{
  switch(unitID)
  {
    case 1:
    {
      Switch_ACZERO_REF_Unit(); 
      Switch_ONE_Unit_Init();
    }break;
    case 2:
    {
      Switch_TWO_Unit_Init();
    }break;
    default:break;
  }
}

/*******************************************************************************
函 数 名:  void SwitchAttrEEPROM_Init(u8 unitID)
功能说明:  执行器单元的初始化
参    数:  unitID-单元号
返 回 值:  无
*******************************************************************************/
void SwitchAttrEEPROM_Init(u8 unitID)
{
  u8 ucUNIT = unitID - 1;
  Switch_Actuator.Sw_Act_Base[ucUNIT].PowerOnState = 0;
  EEPROM_Write(Switch_Actuator.Sw_Act_Base[ucUNIT].PowerOnEEPROMAddr, sizeof(Switch_Actuator.Sw_Act_Base[ucUNIT].PowerOnState), (u8*)&Switch_Actuator.Sw_Act_Base[ucUNIT].PowerOnState); 

}

/*******************************************************************************
函 数 名:  MsgResult_t Set_SwActSwitch_Attr(UnitPara_t *pData)
功能说明:  设置单元一开关属性
参    数:   pData->unit: 	单元号，
            pData->cmd:	  	属性号
            pData->len:     为参数长度
            pData->data:	为参数值(0为关闭，非0为开启)
返 回 值:  	操作结果
*******************************************************************************/
MsgResult_t Set_SwActSwitch_Attr(UnitPara_t *pData)
{
  u8 ucUNIT = pData->unit - 1;
  Switch_Actuator.Sw_Act_Base[ucUNIT].SW_DelayClr();
  if(pData->data[0])
  {
    Switch_Actuator.Sw_Act_Base[ucUNIT].SW_ON();
  }
  else
  {
    Switch_Actuator.Sw_Act_Base[ucUNIT].SW_OFF(); 
  }  
	Upload();	
	return COMPLETE;   
}

/*******************************************************************************
函 数 名:  	MsgResult_t Get_SwActSwitch_Attr(UnitPara_t *pData, u8 *rlen, u8 *rpara)
功能说明:  	获取单元继电器状态
参    数:  	pData->unit: 	单元号，
            pData->cmd:	  	属性号
            pData->len:     为参数长度
            pData->data:	为参数值(0为关闭，非0为开启)
            *rlen-返回参数长度
            *rpara-返回参数存放地址
返 回 值:  	1表示成功，0表示失败
*******************************************************************************/
MsgResult_t Get_SwActSwitch_Attr(UnitPara_t *pData, u8 *rlen, u8 *rpara)
{
	u8 ucUNIT = pData->unit - 1;
	*rlen = 1;
	*rpara = (0x00 == GPIOx_Get(Switch_Actuator.Sw_Act_Base[ucUNIT].pControlPort->ControlPort,Switch_Actuator.Sw_Act_Base[ucUNIT].pControlPort->ControlPin))?(0x00):(0x01);
	return COMPLETE;
}

/*******************************************************************************
函 数 名:  	MsgResult_t Set_SWPowerOnStatus_Attr(UnitPara_t *pData)
功能说明:  	设置单路执行器上电状态
参    数:   pData->unit: 	单元号，
			pData->cmd:	  	属性号
            pData->len:     为参数长度
            pData->data:	为参数值(0为关闭，非0为开启)
返 回 值:  	操作结果
*******************************************************************************/
MsgResult_t Set_SWPowerOnStatus_Attr(UnitPara_t *pData)
{
  MsgResult_t result;
  u8 ucUNIT = pData->unit - 1;
  if (1 == pData->len)	                                /**> 参数长度正确  **/
  {
    if(pData->data[0] > 1)
    {
        result = PARA_MEANING_ERR;
    }
    else
    {
        Switch_Actuator.Sw_Act_Base[ucUNIT].PowerOnState = pData->data[0];
        result = EEPROM_Write(Switch_Actuator.Sw_Act_Base[ucUNIT].PowerOnEEPROMAddr, sizeof(Switch_Actuator.Sw_Act_Base[ucUNIT].PowerOnState), (u8*)&Switch_Actuator.Sw_Act_Base[ucUNIT].PowerOnState);
    }
  }		
  else
  {
      result = PARA_LEN_ERR;
  } 
  return result;
}

/*******************************************************************************
函 数 名:  	MsgResult_t Get_SWPowerOnStatus_Attr(UnitPara_t *pData, u8 *rlen, u8 *rpara)
功能说明:  	获取单路执行器上电状态
参    数:  	pData->unit: 	单元号，
            pData->cmd:	  	属性号
            pData->len:     为参数长度
            pData->data:	为参数值(0为关闭，非0为开启)
            *rlen-返回参数长度
            *rpara-返回参数存放地址
返 回 值:  	1表示成功，0表示失败
*******************************************************************************/
MsgResult_t Get_SWPowerOnStatus_Attr(UnitPara_t *pData, u8 *rlen, u8 *rpara)
{ 
	MsgResult_t result;
	u8 ucUNIT = pData->unit - 1;
	result = EEPROM_Read(Switch_Actuator.Sw_Act_Base[ucUNIT].PowerOnEEPROMAddr, sizeof(Switch_Actuator.Sw_Act_Base[ucUNIT].PowerOnState), (u8*)&Switch_Actuator.Sw_Act_Base[ucUNIT].PowerOnState);
	if (COMPLETE == result)
	{
	*rlen = sizeof(Switch_Actuator.Sw_Act_Base[ucUNIT].PowerOnState);
	memcpy(rpara, &Switch_Actuator.Sw_Act_Base[ucUNIT].PowerOnState, *rlen);
	}
	return result;
}

/*******************************************************************************
函 数 名:  	MsgResult_t Set_SWActReversal_Attr(UnitPara_t *pData)
功能说明:  	墙面开关执行器单元一当前状态取反 
参    数:   pData->unit: 	单元号，
            pData->cmd:	  	属性号
            pData->len:     为参数长度
            pData->data:	为参数值(0为关闭，非0为开启)
返 回 值:  	操作结果
*******************************************************************************/
MsgResult_t Set_SWActReversal_Attr(UnitPara_t *pData)
{
	u8 ucUNIT = pData->unit - 1;
	Switch_Actuator.Sw_Act_Base[ucUNIT].SW_DelayClr();
	Switch_Actuator.Sw_Act_Base[ucUNIT].SW_REV();
	Upload();	
	return COMPLETE;
}

/*******************************************************************************
函 数 名:  	MsgResult_t Set_SWActionTimer_Attr(UnitPara_t *pData)
功能说明:  	设置开关动作属性
参    数:   pData->unit: 	单元号，
            pData->cmd:	  	属性号
            pData->len:     为参数长度
            pData->data:	为参数值(0为关闭，非0为开启)
返 回 值:  	操作结果
*******************************************************************************/
MsgResult_t Set_SWActionTimer_Attr(UnitPara_t *pData)
{
	u8 ucUNIT = pData->unit - 1;
	if (pData->len <10)
	{
		return PARA_LEN_ERR;
	}
	Switch_Actuator.Sw_Act_Base[ucUNIT].SW_DelayClr();
	Switch_Actuator.Sw_Act_Base[ucUNIT].SwActTime.firstTime = ((*(u32*)&pData->data[1])) * 1000;
	Switch_Actuator.Sw_Act_Base[ucUNIT].SwActTime.secondTime= ((*(u32*)&pData->data[5])) * 1000;
	Switch_Actuator.Sw_Act_Base[ucUNIT].SwActTime.cycleFlag = pData->data[9];
	Switch_Actuator.Sw_Act_Base[ucUNIT].SwActTime.fun       = (pData->data[0] == 0 ? (Switch_Actuator.Sw_Act_Base[ucUNIT].SW_OFF) : (Switch_Actuator.Sw_Act_Base[ucUNIT].SW_ON));
	Thread_Login(ONCEDELAY, 0, 	Switch_Actuator.Sw_Act_Base[ucUNIT].SwActTime.firstTime, Switch_Actuator.Sw_Act_Base[ucUNIT].SW_DelayOpt1);
	return COMPLETE;
}

/*******************************************************************************
函 数 名:  	MsgResult_t Set_SW_ZeroCross_Attr_Enable(UnitPara_t *pData)
功能说明:  	设置继电器器过零检测使能
参    数:   pData->unit: 	单元号，
            pData->cmd:	  	属性号
            pData->len:     为参数长度
            pData->data:	为参数值(0为关闭，非0为开启)
返 回 值:  	操作结果
*******************************************************************************/
MsgResult_t Set_SW_ZeroCross_Attr_Enable(UnitPara_t *pData)
{
	u8 ucUNIT = pData->unit - 1;
	if (pData->len < 2)
	{
		return PARA_LEN_ERR;
	}
	Switch_Actuator.Sw_Act_Base[ucUNIT].SW_CrossZero.CheckFlagON = pData->data[0];
	Switch_Actuator.Sw_Act_Base[ucUNIT].SW_CrossZero.CheckFlagOFF= pData->data[1];
	return COMPLETE;
}

/*******************************************************************************
函 数 名:  	MsgResult_t Get_SW_ZeroCross_Attr_Enable(UnitPara_t *pData, u8 *rlen, u8 *rpara)
功能说明:  	获取继电器器过零检测使能状态
参    数:   pData->unit: 	单元号，
            pData->cmd:	  	属性号
            pData->len:     为参数长度
            pData->data:	为参数值(0为关闭，非0为开启)
返 回 值:  	操作结果
*******************************************************************************/
MsgResult_t Get_SW_ZeroCross_Attr_Enable(UnitPara_t *pData, u8 *rlen, u8 *rpara)
{
	u8 ucUNIT = pData->unit - 1;
	*rlen  = 2;
	*rpara = Switch_Actuator.Sw_Act_Base[ucUNIT].SW_CrossZero.CheckFlagON;
	*(rpara+1) = Switch_Actuator.Sw_Act_Base[ucUNIT].SW_CrossZero.CheckFlagOFF;
	return COMPLETE;
}

/*******************************************************************************
函 数 名:  	MsgResult_t Get_SW_ZeroCross_Attr(UnitPara_t *pData, u8 *rlen, u8 *rpara)
功能说明:  	获取单路执行器上电状态
参    数:  	pData->unit: 	单元号，
            pData->cmd:	  	属性号
            pData->len:     为参数长度
            pData->data:	为参数值(0为关闭，非0为开启)
            *rlen-返回参数长度
            *rpara-返回参数存放地址
返 回 值:  	1表示成功，0表示失败
*******************************************************************************/
MsgResult_t Get_SW_ZeroCross_Attr(UnitPara_t *pData, u8 *rlen, u8 *rpara)
{
	u8 ucUNIT = pData->unit - 1;
	(*(u16*)(rpara))    = Switch_Actuator.Sw_Act_Base[ucUNIT].SW_CrossZero.CrossZeroTimeON;         /**< 继电器闭合时间 */
	(*(u16*)(rpara+2))  = Switch_Actuator.Sw_Act_Base[ucUNIT].SW_CrossZero.CrossZeroTimeOFF;        /**< 继电器断开时间 */  
	*rlen = 4;	
	return  COMPLETE;
}

/*******************************************************************************
函 数 名:  	void SWActInstruct_Child(void)
功能说明:  	用于设备验证
参    数:  	unit:	对应的指示单元
返 回 值:  	无
注    意:	  当unit = 0 时设备恢复到非提示状态
*******************************************************************************/
static void SWActInstruct_Child(void)
{
	if(1 == Key_OPT[0].BindingState)
	{
		LED_OPT[0].LED_REV();
	}	
	if(1 == Key_OPT[1].BindingState)
	{
		LED_OPT[1].LED_REV();
	}
	LED_OPT[2].LED_REV();
	Switch_Actuator.Sw_Act_Base[0].SW_REV();
	Switch_Actuator.Sw_Act_Base[1].SW_REV();
}

/*******************************************************************************
函 数 名:  	void SWActInstruct(void)
功能说明:  	验证单元指示
参    数:  	unit:	对应的指示单元
返 回 值:  	无
注    意:	  当unit = 0 时设备恢复到非提示状态
*******************************************************************************/
void SWActInstruct(void)
{
	if(1 == Key_OPT[0].BindingState)
	{
		LED_OPT[0].LED_OFF();
	}
	Switch_Actuator.Sw_Act_Base[0].SW_OFF();
	if(1 == Key_OPT[0].BindingState)
	{
		LED_OPT[1].LED_OFF();
	}
	Switch_Actuator.Sw_Act_Base[1].SW_OFF();  
	LED_OPT[2].LED_OFF();
	Thread_Login(MANY, 6, 1000, &SWActInstruct_Child);		
}

/**********************************End of File*********************************/

