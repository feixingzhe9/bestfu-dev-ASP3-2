/***************************Copyright BestFu 2014-05-14*************************
文	件：    SysExeAttr.h
说	明：    系统级属性命令执行表，所有命令的实现
编	译：    Keil uVision4 V4.54.0.0
版	本：    v2.0
编	写：    Unarty
日	期：    2014.06.26
修　改：	暂无
*******************************************************************************/
#ifndef	__SYS_EXE_ATTR_H
#define	__SYS_EXE_ATTR_H


/*********************************变量声明*************************************/
extern SysData_t gSysData;
extern const AttrExe_st SysAttrTable[];

/*********************************函数声明*************************************/
void SysAttr_Init(BF_INT08U unitID);
BF_INT08U Sys_UnitAttrNum(void);
void SysEEPROM_Init(UnitPara_t unitID);
MsgResult_t Set_UserDataFormat_Attr(UnitPara_t *pData);
MsgResult_t Get_DeviceType_Attr(UnitPara_t *pData, BF_INT08U *rlen, BF_INT08U *rpara);
MsgResult_t Get_Version_Attr(UnitPara_t *pData, BF_INT08U *rlen, BF_INT08U *rpara);

#endif
/**************************Copyright BestFu 2014-05-14*************************/
