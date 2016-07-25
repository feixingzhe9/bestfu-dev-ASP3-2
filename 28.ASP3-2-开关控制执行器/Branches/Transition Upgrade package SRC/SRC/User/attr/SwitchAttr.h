/***************************Copyright BestFu 2015-08-03*************************
文 件:	SwitchAttr.h
说 明:	双路开关执行器设备属性实现文件
编 辑:	Keil uVision5 V5.12
版 本:	v1.0
编 写:	Jay
日 期:	2015.08.03
修 改:	暂无
*******************************************************************************/
#ifndef __SWITCHATTR_H
#define __SWITCHATTR_H

#include "UnitCfg.h"

/**********************************变量声明*********************************/

extern const AttrExe_st SwitchAttrTable[];

/******************************************************************************/
void SwitchAttr_Init(u8 unitID);
void SwitchAttrEEPROM_Init(u8 unitID);

MsgResult_t Set_SWActionTimer_Attr(UnitPara_t *pData);

MsgResult_t Set_SWActReversal_Attr(UnitPara_t *pData);

MsgResult_t Get_SWPowerOnStatus_Attr(UnitPara_t *pData, u8 *rlen, u8 *rpara);
MsgResult_t Set_SWPowerOnStatus_Attr(UnitPara_t *pData);

MsgResult_t Get_SwActSwitch_Attr(UnitPara_t *pData, u8 *rlen, u8 *rpara);
MsgResult_t Set_SwActSwitch_Attr(UnitPara_t *pData);

MsgResult_t Get_SW_ZeroCross_Attr(UnitPara_t *pData, u8 *rlen, u8 *rpara);
MsgResult_t Set_SW_ZeroCross_Attr_Enable(UnitPara_t *pData);
MsgResult_t Get_SW_ZeroCross_Attr_Enable(UnitPara_t *pData, u8 *rlen, u8 *rpara);
void SWActInstruct(void);
#endif

