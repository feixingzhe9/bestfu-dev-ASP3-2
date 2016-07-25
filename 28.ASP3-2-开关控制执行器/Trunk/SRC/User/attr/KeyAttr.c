/***************************Copyright BestFu 2015-11*************************
文 件:    KeyAttr.c
说 明:    智慧开关设备属性实现文件
编 辑:    Keil uVision5 V5.12
版 本:    v1.0
编 写:    Jay
日 期:    2015.11.02
修 改:    暂无
*******************************************************************************/
#include "Thread.h"
#include "KeyAttr.h"
#include "KeyUnit.h"
#include "ledunit.h"
#include "SwitchUnit.h"
#include "relay.h"


//对属性读写的接口列表
const AttrExe_st KeyAttrTable[] =
{
    {0x01, LEVEL_1, NULL                            , Get_TouchKeyAction_Attr         ,0},
    {0x02, LEVEL_1, Set_TouchKeyState_Attr          , Get_TouchKeyState_Attr          ,0},
    {0x03, LEVEL_1, NULL                            , Get_TouchKeyAction_Attr         ,0},
    {0x04, LEVEL_1, Set_TouchKeyState_Attr          , Get_TouchKeyState_Attr          ,0},
    {0x05, LEVEL_1, NULL                            , Get_TouchKeyAction_Attr         ,0},
    {0x06, LEVEL_1, Set_TouchKeyState_Attr          , Get_TouchKeyState_Attr          ,0},
    {0x07, LEVEL_0, Set_RelayBangdingKey_Attr_One   , Get_RelayBangdingKey_Attr_One   ,0},
    {0x08, LEVEL_0, Set_RelayBangdingKey_Attr_Two   , Get_RelayBangdingKey_Attr_Two   ,0},
    {0x09, LEVEL_0, Set_LEDLogicState_Attr_One      , Get_LEDLogicState_Attr_One      ,0},
    {0x0a, LEVEL_0, Set_LEDLogicState_Attr_Two      , Get_LEDLogicState_Attr_Two      ,0}
};

/*******************************************************************************
函 数 名:  Key_UnitAttrNum
功能说明:  计算按键单元属性数量
参    数:  无
返 回 值:  无
*******************************************************************************/
u8 Key_UnitAttrNum(void)
{
    return (BF_DIM(KeyAttrTable));
}

/*******************************************************************************
函 数 名:  void KeyAttr_Init(u8 unitID)
功能说明:  按键单元的初始化
参    数:  unitID-单元号
返 回 值:  无
*******************************************************************************/
void KeyAttr_Init(u8 unitID)
{
    Key_Unit_Init();                            /**> 按键灯初始化 **/
    LED_UNIT_Init();                            /**> LED灯初始化  **/
    Thread_Login(FOREVER, 0, 5, &Key_Handle);
}

/*******************************************************************************
**函    数： KeyEepromInit
**功    能： 所属 eeprom 初始化
**参    数： unitID      --单元号
**返    回： void
*******************************************************************************/
void KeyEepromInit(u8 unitID)
{
    Key_OPT[0].BindingState = 0;
    Key_OPT[0].LEDLogicState = 0;
    EEPROM_Write(Key_OPT[0].BindingEEPROMAddr, sizeof(Key_OPT[0].BindingState) + sizeof(Key_OPT[0].LEDLogicState), (u8 *)&Key_OPT[0].BindingState);

    Key_OPT[1].BindingState = 0;
    Key_OPT[1].LEDLogicState = 0;
    EEPROM_Write(Key_OPT[1].BindingEEPROMAddr, sizeof(Key_OPT[1].BindingState) + sizeof(Key_OPT[1].LEDLogicState), (u8 *)&Key_OPT[1].BindingState);
}

/*******************************************************************************
**函    数： MsgResult_t Get_TouchKeyAction_Attr(UnitPara_t *pData, u8 *rLen, u8 *rData)
**功    能： 获取触摸按键动作状态值
**参    数： *pData      --输入参数
             *rLen       --返回参数长度
             *rData      --返回参数存放地址
**返    回： TRUE/FALSE
********************************************************************************/
MsgResult_t Get_TouchKeyAction_Attr(UnitPara_t *pData, u8 *rLen, u8 *rData)
{
    switch (pData->cmd)
    {
    case 0x01:
    {
        rData[0] = Key_OPT[0].KeyAct.Status;
    }
    break;
    case 0x03:
    {
        rData[0] = Key_OPT[1].KeyAct.Status;
    }
    break;
    case 0x05:
    {
        rData[0] = Key_OPT[2].KeyAct.Status;
    }
    break;
    default:
        break;
    }
    *rLen    = 1;
    return COMPLETE;
}

/*******************************************************************************
函 数 名:    MsgResult_t Set_TouchKeyState_Attr(UnitPara_t *pData)
功能说明:   设置触摸按键动作状态值
参    数:   pData->unit:  单元号，
            pData->cmd:     属性号
            pData->len:     为参数长度
            pData->data:    为参数值(0为关闭，非0为开启)
返 回 值:    操作结果
*******************************************************************************/
MsgResult_t Set_TouchKeyState_Attr(UnitPara_t *pData)
{
    if (pData->len < 1)
    {
        return PARA_LEN_ERR;
    }
    if (pData->data[0] > 1)
    {
        return PARA_MEANING_ERR;
    }
    switch (pData->cmd)
    {
    case 0x02:
    {
        if (1 == Key_OPT[0].BindingState)//在绑定状态下,按键的状态值与开关的状态值一致
        {
            Key_OPT[0].KeySta.Status = *((KEY_TYPE_t *)(&pData->data[0]));
        }
        else
        {
            return PARA_MEANING_ERR;
        }
    }
    break;
    case 0x04:
    {
        if (1 == Key_OPT[1].BindingState)//在绑定状态下,按键的状态值与开关的状态值一致
        {
            Key_OPT[1].KeySta.Status = *((KEY_TYPE_t *)(&pData->data[0]));
        }
        else
        {
            return PARA_MEANING_ERR;
        }
    }
    break;
    case 0x06:
    {
        Key_OPT[2].KeySta.Status = *((KEY_TYPE_t *)(&pData->data[0]));
    }
    break;
    default:
        break;
    }
    return COMPLETE;
}

/*******************************************************************************
**函    数： MsgResult_t Get_TouchKeyState_Attr(UnitPara_t *pData, u8 *rLen, u8 *rData)
**功    能： 获取触摸按键状态值
**参    数： *pData      --输入参数
             *rLen       --返回参数长度
             *rData      --返回参数存放地址
**返    回： TRUE/FALSE
********************************************************************************/
MsgResult_t Get_TouchKeyState_Attr(UnitPara_t *pData, u8 *rLen, u8 *rData)
{
    switch (pData->cmd)
    {
    case 0x02:
    {
        if (0 == Key_OPT[0].BindingState)
        {
            rData[0] = (0 == (GPIOx_Get(Switch_Actuator.Sw_Act_Base[0].pControlPort->ControlPort, Switch_Actuator.Sw_Act_Base[0].pControlPort->ControlPin))) ? (SWITCH_OFF) : (SWITCH_ON);
        }
        else
        {
            rData[0] = Key_OPT[0].KeySta.Status;
        }
    }
    break;
    case 0x04:
    {
        if (0 == Key_OPT[1].BindingState)
        {
            rData[0] = (0 == (GPIOx_Get(Switch_Actuator.Sw_Act_Base[1].pControlPort->ControlPort, Switch_Actuator.Sw_Act_Base[1].pControlPort->ControlPin))) ? (SWITCH_OFF) : (SWITCH_ON);
        }
        else
        {
            rData[0] = Key_OPT[1].KeySta.Status;
        }
    }
    break;
    case 0x06:
    {
        rData[0] = Key_OPT[2].KeySta.Status;
    }
    break;
    default:
        break;
    }

    *rLen    = 1;

    return COMPLETE;
}

/*******************************************************************************
函 数 名:    MsgResult_t Set_RelayBangdingKey_Attr_One(UnitPara_t *pData)
功能说明:   设置继电器器与按键绑定关系
参    数:   pData->unit:  单元号，
            pData->cmd:     属性号
            pData->len:     为参数长度
            pData->data:    为参数值(0为关闭，非0为开启)
返 回 值:    操作结果
*******************************************************************************/
MsgResult_t Set_RelayBangdingKey_Attr_One(UnitPara_t *pData)
{
    if (pData->len < 1)
    {
        return PARA_LEN_ERR;
    }
    if (pData->data[0] > 1)
    {
        return PARA_MEANING_ERR;
    }
    Key_OPT[0].BindingState = pData->data[0];

    return EEPROM_Write(Key_OPT[0].BindingEEPROMAddr, sizeof(Key_OPT[0].BindingState), (u8 *) & (Key_OPT[0].BindingState));
}

/*******************************************************************************
函 数 名:    MsgResult_t Get_RelayBangdingKey_Attr_One(UnitPara_t *pData, u8 *rlen, u8 *rpara)
功能说明:   获取继电器器与按键绑定关系
参    数:   pData->unit:  单元号，
            pData->cmd:     属性号
            pData->len:     为参数长度
            pData->data:    为参数值(0为关闭，非0为开启)
返 回 值:    操作结果
*******************************************************************************/
MsgResult_t Get_RelayBangdingKey_Attr_One(UnitPara_t *pData, u8 *rlen, u8 *rpara)
{
    *rlen  = 1;
    *rpara = Key_OPT[0].BindingState;

    return COMPLETE;
}

/*******************************************************************************
函 数 名:    MsgResult_t Set_RelayBangdingKey_Attr_Two(UnitPara_t *pData)
功能说明:   设置继电器器与按键绑定关系
参    数:   pData->unit:  单元号，
            pData->cmd:     属性号
            pData->len:     为参数长度
            pData->data:    为参数值(0为关闭，非0为开启)
返 回 值:    操作结果
*******************************************************************************/
MsgResult_t Set_RelayBangdingKey_Attr_Two(UnitPara_t *pData)
{
    if (pData->len < 1)
    {
        return PARA_LEN_ERR;
    }
    if (pData->data[0] > 1)
    {
        return PARA_MEANING_ERR;
    }
    Key_OPT[1].BindingState = pData->data[0];

    return EEPROM_Write(Key_OPT[1].BindingEEPROMAddr, sizeof(Key_OPT[1].BindingState), (u8 *) & (Key_OPT[1].BindingState));
}

/*******************************************************************************
函 数 名:    MsgResult_t Get_RelayBangdingKey_Attr_Two(UnitPara_t *pData, u8 *rlen, u8 *rpara)
功能说明:   获取继电器器与按键绑定关系
参    数:   pData->unit:  单元号，
            pData->cmd:     属性号
            pData->len:     为参数长度
            pData->data:    为参数值(0为关闭，非0为开启)
返 回 值:    操作结果
*******************************************************************************/
MsgResult_t Get_RelayBangdingKey_Attr_Two(UnitPara_t *pData, u8 *rlen, u8 *rpara)
{
    *rlen  = 1;
    *rpara = Key_OPT[1].BindingState;

    return COMPLETE;
}

/*******************************************************************************
函 数 名:    MsgResult_t Set_LEDLogicState_Attr_One(UnitPara_t *pData)
功能说明:   设置继电器器与按键时LED的逻辑状态
参    数:   pData->unit:  单元号，
            pData->cmd:     属性号
            pData->len:     为参数长度
            pData->data:    为参数值(0为关闭，非0为开启)
返 回 值:    操作结果
*******************************************************************************/
MsgResult_t Set_LEDLogicState_Attr_One(UnitPara_t *pData)
{
    if (pData->len < 1)
    {
        return PARA_LEN_ERR;
    }
    if (pData->data[0] > 1)
    {
        return PARA_MEANING_ERR;
    }
    Key_OPT[0].LEDLogicState = pData->data[0];

    return EEPROM_Write(Key_OPT[0].BindingEEPROMAddr + sizeof(Key_OPT[0].BindingState), sizeof(Key_OPT[0].LEDLogicState), (u8 *)&Key_OPT[0].LEDLogicState);
}

/*******************************************************************************
函 数 名:    MsgResult_t Get_RelayBangdingKeyAttr_One(UnitPara_t *pData, u8 *rlen, u8 *rpara)
功能说明:   获取继电器器与按键时LED的逻辑状态
参    数:   pData->unit:  单元号，
            pData->cmd:     属性号
            pData->len:     为参数长度
            pData->data:    为参数值(0为关闭，非0为开启)
返 回 值:    操作结果
*******************************************************************************/
MsgResult_t Get_LEDLogicState_Attr_One(UnitPara_t *pData, u8 *rlen, u8 *rpara)
{
    *rlen  = 1;
    *rpara = Key_OPT[0].LEDLogicState;

    return COMPLETE;
}

/*******************************************************************************
函 数 名:    MsgResult_t Set_LEDLogicState_Attr_Two(UnitPara_t *pData)
功能说明:   设置继电器器与按键时LED的逻辑状态
参    数:   pData->unit:  单元号，
            pData->cmd:     属性号
            pData->len:     为参数长度
            pData->data:    为参数值(0为关闭，非0为开启)
返 回 值:    操作结果
*******************************************************************************/
MsgResult_t Set_LEDLogicState_Attr_Two(UnitPara_t *pData)
{
    if (pData->len < 1)
    {
        return PARA_LEN_ERR;
    }
    if (pData->data[0] > 1)
    {
        return PARA_MEANING_ERR;
    }
    Key_OPT[1].LEDLogicState = pData->data[0];

    return EEPROM_Write(Key_OPT[1].BindingEEPROMAddr + sizeof(Key_OPT[1].BindingState), sizeof(Key_OPT[1].LEDLogicState), (u8 *)&Key_OPT[1].LEDLogicState);
}

/*******************************************************************************
函 数 名:    MsgResult_t Get_RelayBangdingKeyAttr_Two(UnitPara_t *pData, u8 *rlen, u8 *rpara)
功能说明:   获取继电器器与按键时LED的逻辑状态
参    数:   pData->unit:  单元号，
            pData->cmd:     属性号
            pData->len:     为参数长度
            pData->data:    为参数值(0为关闭，非0为开启)
返 回 值:    操作结果
*******************************************************************************/
MsgResult_t Get_LEDLogicState_Attr_Two(UnitPara_t *pData, u8 *rlen, u8 *rpara)
{
    *rlen  = 1;
    *rpara = Key_OPT[1].LEDLogicState;

    return COMPLETE;
}

/*********************************End of File*********************************/

