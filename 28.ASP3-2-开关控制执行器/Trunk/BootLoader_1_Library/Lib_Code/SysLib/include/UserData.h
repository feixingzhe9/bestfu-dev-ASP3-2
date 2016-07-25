/***************************Copyright BestFu 2014-05-14*************************
文	件：	UserData.h
说	明：	用户数据管理文件
编	译：	Keil uVision4 V4.54.0.0
版	本：	v1.0
编	写：	Unarty
日	期：	2014.06.24 
修　改：	暂无

*******************************************************************************/
#ifndef __USERDATA_H
#define __USERDATA_H

/*********************************自定义宏*************************************/
#define DEFAULT_USERID				(0xFFFFFFFF)							//默认用户ID号
#define SUPER_USERID				(0x54534542)							//超级用户ID号("BEST")
#define DEFAULT_AREA				(0xFFFFFFFF)							//默认区域号
#define UNIT_MAX_NUM				(UNITDATA_SPACE/ONCEUNITDATA_SPACE)		//容许最大单元数
#define AREA_RANK_NUM				(4)										//区域分级数

#define USERDATA_START_ADD			(0x0000)								//用户数据存储起始地址
#define USERDATA_END_ADD			(0x3000)								//用户数据存储结束地址(8K)

/*系统异常次数EEPROM记录空间*/
#define FAULT_START_ADD				(USERDATA_START_ADD)					//异常存储首地址
#define FAULT_SPACE					(0x40)									//异常数据占用空间大小 

/*系统数据EEPROM空间分配*/
#define SYSDATA_START_ADD			(FAULT_START_ADD + FAULT_SPACE)			//系统数据存储首地址
#define SYSDATA_SPACE				(0x40)									//系统数据占用空间大小 
#define SYSDATA_RESEVER_SPACE		(SYSDATA_SPACE - sizeof(SysData_t))		//系统数据剩余空间

/*单元数据EEPROM空间分配*/
#define UNITDATA_START_ADD			(SYSDATA_START_ADD + SYSDATA_SPACE)		//单元数据存储首地址 
#define UNITDATA_SPACE				(0x180)									//用户数据占用空间大小 
#define ONCEUNITDATA_SPACE			(0x10)									//单个用户单元数据空间
#define UNITDATA_RESEVER_SPACE		(UNITDATA_SPACE - sizeof(UnitData_t))	//用户数据剩余空间

/*********************************数据类型定义*********************************/
#pragma pack(1)     						//按1字节对齐
typedef struct								//系统共用数据区域
{
	BF_INT32U 	userID;						//用户号
	BF_INT32U 	deviceID;					//设备ID
	BF_INT08U 	sendCnt;					//数据发送次数
	BF_INT08U  	serialNum;  				//流水号
	BF_INT32U 	BandAddr[5];				//转发绑定地址
	BF_INT08U  	BandAreaEn;					//转发绑定区域广播指令使能标志
}SysData_t;									//系统共用数据区

typedef struct 								//单元数据区域
{
	BF_INT08U 	area[4];					//区域号
	BF_INT16U 	type;						//所属类型
	BF_INT08U 	able;						//单元使能标志位
	BF_INT08U   common;						//单元共用大类
}UnitData_t;

typedef enum 
{
	FristLevel = 1,
	SecondLevel = 2,
}DataInitLevel_t;

/*********************************变量声明*************************************/
extern SysData_t gSysData;

/*********************************函数声明*************************************/
void UserData_Init(void);
void UserEEPROMData_Init(DataInitLevel_t level);

/*系统EEPROM数据操作*/
void SysEEPROMData_Init(void);
BF_INT08U SysEEPROMData_Set(BF_INT08U size , BF_INT08U *data);

///*单元共用EEPROM数据操作*/
void UnitPublicEEPROMData_Init(void);
void UnitPublicEEPROMUnitData_Init(BF_INT08U unit);
void UnitPublicEEPROMData_Get(BF_INT08U unitCnt, UnitData_t *unitData);
BF_INT08U UnitPublicEEPROMData_Set(BF_INT08U size , BF_INT08U *data);

///*单元共用EEPROM数据操作*/
void UnitPrivateEEPROMData_Init(void);
void UnitPrivateEEPROMUnitData_Init(BF_INT08U unit);

#endif // UserData.h end 
/**************************Copyright BestFu 2014-05-14*************************/
