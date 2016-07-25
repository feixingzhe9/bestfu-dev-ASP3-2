/***************************Copyright BestFu 2015-08-03*************************
文 件:	SwitchAttr.h
说 明:	智慧开关设备属性实现文件
编 辑:	Keil uVision5 V5.12
版 本:	v1.0
编 写:	lsc
日 期:	2015.08.03
修 改:	暂无
*******************************************************************************/
#ifndef __KEYATTR_H
#define __KEYATTR_H

#include "UnitCfg.h"

extern const AttrExe_st KeyAttrTable[];

/******************************************************************************/
void KeyAttr_Init(u8 unitID);
void KeyEepromInit(u8 unitID);

MsgResult_t Get_TouchKeyAction_Attr(UnitPara_t *pData, u8 *rLen, u8 *rData);
MsgResult_t Set_TouchKeyState_Attr(UnitPara_t *pData);
MsgResult_t Get_TouchKeyState_Attr(UnitPara_t *pData, u8 *rLen, u8 *rData);

MsgResult_t Set_RelayBangdingKey_Attr_One(UnitPara_t *pData);
MsgResult_t Get_RelayBangdingKey_Attr_One(UnitPara_t *pData, u8 *rlen, u8 *rpara);
MsgResult_t Set_RelayBangdingKey_Attr_Two(UnitPara_t *pData);
MsgResult_t Get_RelayBangdingKey_Attr_Two(UnitPara_t *pData, u8 *rlen, u8 *rpara);
MsgResult_t Set_LEDLogicState_Attr_One(UnitPara_t *pData);
MsgResult_t Get_LEDLogicState_Attr_One(UnitPara_t *pData, u8 *rlen, u8 *rpara);
MsgResult_t Set_LEDLogicState_Attr_Two(UnitPara_t *pData);
MsgResult_t Get_LEDLogicState_Attr_Two(UnitPara_t *pData, u8 *rlen, u8 *rpara);

void KEY_ActInstruct(void);
/******************************************************************************/
#endif
