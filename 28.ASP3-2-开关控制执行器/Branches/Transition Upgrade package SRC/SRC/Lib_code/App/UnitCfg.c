/***************************Copyright BestFu 2014-05-14*************************
文	件：	UnitCfg.c
说	明：	单元模块配置源代码
编	译：	MDK-ARM Professional Versoin: 5.12.0.0
版	本：	v1.0
编	写：	Unarty
日	期：	2014.07.14 
修　改：	暂无
*******************************************************************************/
#include "UnitCfg.h"
#include "UserData.h"


#define UNIT_CNT	(sizeof(UnitTab)/sizeof(Unit_st))		//总单元数

/*添加: 包含单元属性头文件*/
#include "SysExeAttr.h"
#include "SwitchAttr.h"
#include "KeyAttr.h"

/*添加单元接口函数，与属性接口列表*/
/*格式：｛上电初始化函数接口，EEPROM数据初始化函数接口，单元验证函数接口，单元属性列表名｝*/
/*以上接口函数，当单元不需要此功能时直接用NULL表示。单元属性列表名必须为一个确定*/
const Unit_st UnitTab[] = 
{
    {SysAttr_Init	    , NULL				    , NULL			   ,      (AttrExe_st*)SysAttrTable	},	//系统共用属性，添加者请误修改
    {SwitchAttr_Init    , SwitchAttrEEPROM_Init , SWActInstruct    , (AttrExe_st*)SwitchAttrTable   },  
    {SwitchAttr_Init    , SwitchAttrEEPROM_Init , SWActInstruct    , (AttrExe_st*)SwitchAttrTable   },  
    {KeyAttr_Init    	, KeyEepromInit         , SWActInstruct    , (AttrExe_st*)KeyAttrTable    	}  
};



UnitData_t gUnitData[UNIT_CNT];

/*******************************************************************************
函 数 名：	UnitCnt_Get
功能说明： 	获取当前单元总数
参	  数： 	无
返 回 值：	当前单元数
*******************************************************************************/
u8 UnitCnt_Get(void)
{
	return ((sizeof(UnitTab)/sizeof(Unit_st)));
}

/*******************************************************************************
函 数 名：	Unit_Init
功能说明： 	初始化所有单元
参	  数： 	无
返 回 值：	无
*******************************************************************************/
void Unit_Init(void)
{
	u8 data[250];
	u8 i = UNIT_CNT;
	
	EEPROM_Read(0x6000, sizeof(data), data);
	UnitPublicEEPROMData_Get(UNIT_CNT, gUnitData);	//获取用户设置的单元属性值
	
	while (i)	
	{
		if (UnitTab[--i].Init != NULL)
		{
			UnitTab[i].Init(i);		//初始化各单元数据
		}
	}
}

/**************************Copyright BestFu 2014-05-14*************************/
