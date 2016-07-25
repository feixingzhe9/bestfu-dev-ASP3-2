/***************************Copyright BestFu 2014-05-14*************************
文	件：	UnitCgf.h
说	明：	单元模块配置头文件
编	译：	Keil uVision4 V4.54.0.0
版	本：	v1.0
编	写：	Unarty
日	期：	2014.07.14 
修　改：	暂无
*******************************************************************************/
#ifndef __UNITCFG_H
#define __UNITCFG_H

/*********************************数据声明*************************************/
extern const Unit_st UnitTab[];
extern UnitData_t gUnitData[];

/*********************************函数声明*************************************/
void Unit_Init(void);
BF_INT08U UnitCnt_Get(void);

/*用户EEPROM数据读写操作函数*/
extern MsgResult_t EEPROM_Write(BF_INT32U addr, BF_INT32U len, BF_INT08U *data);
extern MsgResult_t EEPROM_Read(BF_INT32U addr, BF_INT32U len, BF_INT08U *data);

#endif
/**************************Copyright BestFu 2014-05-14*************************/
