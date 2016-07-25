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

#include "Bf_type.h"

/**************************系统EEPROM存储定义*********************************/
#define SYS_VERSION					(10200)											//系统库版本, UnartyChen Check

#define DEFAULT_USERID				(0xFFFFFFFF)									//默认用户ID号
#define SUPER_USERID				(0x54534542)									//超级用户ID号("BEST")
#define DEFAULT_AREA				(0xFFFFFFFF)									//默认区域号
#define UNIT_MAX_NUM				(UNITDATA_SPACE/ONCEUNITDATA_SPACE)				//容许最大单元数
#define AREA_RANK_NUM				(4)												//区域分级数

#define SCENCETIME_EN  				(1u)											//配置场景延时执行功能

#define USERDATA_START_ADD			(0x0000)										//用户数据存储起始地址
#define USERDATA_END_ADD			(0x3000)										//用户数据存储结束地址(8K)

/*系统异常次数EEPROM记录空间*/
#define FAULT_START_ADD				(USERDATA_START_ADD)							//异常存储首地址
#define FAULT_SPACE					(0x40)											//异常数据占用空间大小 

/*系统数据EEPROM空间分配*/
#define SYSDATA_START_ADD			(FAULT_START_ADD + FAULT_SPACE)					//系统数据存储首地址
#define SYSDATA_SPACE				(0x40)											//系统数据占用空间大小 
#define SYSDATA_RESEVER_SPACE		(SYSDATA_SPACE - sizeof(SysData_t))				//系统数据保留空间
																
/*单元数据EEPROM空间分配*/
#define UNITDATA_START_ADD			(SYSDATA_START_ADD + SYSDATA_SPACE)				//单元数据存储首地址 
#define UNITDATA_SPACE				(0x180)											//用户数据占用空间大小 
#define ONCEUNITDATA_SPACE			(0x10)											//单个用户单元数据空间
#define UNITDATA_RESEVER_SPACE		(UNITDATA_SPACE - sizeof(UnitData_t))			//用户数据剩余空间

/*单元组数据EEPROM空间分配*/
#define GROUP_START_ADD				(UNITDATA_START_ADD + UNITDATA_SPACE)			//设备组组数据存储地址(起始地址0x200)
#define GROUP_SPACE					(0x400)											//设备组占用空间大小
//#define GROUP_RESEVER_SPACE			(GROUP_SPACE-UNIT_GROUP_SPACE*UNIT_MAX_NUM)		//组剩余空间大小(预留每单元400个组的数量)
#define UNIT_GROUP_NUM				(320)											//每个单元组的总数
#define AREA_GROUP_NUM				(64)											//每个区域组的总数
#define PER_GROUP_SPACE				(1.0/8)											//组占用空间
#define UNIT_GROUP_SPACE			(UNIT_GROUP_NUM*PER_GROUP_SPACE)				//每个单元组占用的存储空间
#define AREA_GROUP_SPACE			(AREA_GROUP_NUM*PER_GROUP_SPACE)				//每个区域组占用的存储空间

/*单元场景数据EEPROM空间分配*/
#define SCENE_START_ADD				(GROUP_START_ADD + GROUP_SPACE)					//场景起始地址(起始地址0x600)
#define SCENE_SPACE					(0x1E00)										//场景空间(7.5K,支持256个场景,每级区域下场景数不限)
#define SCENE_END_ADD				(SCENE_START_ADD + SCENE_SPACE)					//场景EEPROM结束地址
#define AREA_SCENE_NUM				(64)											//每个区域场景标识总数

#define SCENE_INDEX_START_ADD		(SCENE_START_ADD)								//设备场景索引数据存储地址	
#define SCENE_INDEX_SPACE			(0x600)											//设备场景索引空间存储大小(支持3个扩展)
#define SCENE_DEFAULT_VALUE			(0x00)											//场景标识默认值

/*场景内存使用标识存储地址分配*/
#define SCENE_USE_FLAG_ADD			(SCENE_START_ADD + SCENE_INDEX_SPACE)			//场景内存使用标记地址(起始地址0xC00)
#define SCENE_USE_FLAG_TOTAL_SPACE	(0x80)											//场景内存使用标记总占用空间大小
#define SCENE_USE_SPACE				(0x20)											//场景内存表示占用空间大小
#if (SCENCETIME_EN > 0u)
#define SCENE_TIME_USE_FLAG_ADD		(SCENE_USE_FLAG_ADD + SCENE_USE_SPACE)			//场景时间内存使用标记地址	
#define SCENE_TIME_USE_SPACE		(SCENE_TIME_TOTAL_NUM/8)						//场景时间内存表示占用空间大小
#else
#define SCENE_TIME_USE_SPACE		(0)												//场景时间内存表示占用空间大小
#endif

/*场景数据存储地址*/
#define SCENE_MAX_NUM				(SCENE_SPACE1_NUM +\
									 SCENE_SPACE2_NUM +\
									 SCENE_SPACE3_NUM)								//最大场景数
#define SCENE_DATA_START_ADD		(SCENE_USE_FLAG_ADD+SCENE_USE_FLAG_TOTAL_SPACE)	//场景数据存储位置的起始地址(起始地址0xC80)
#define SCENE_DATA_SPACE			(0x1000)										//场景数据存储空间,4K
#define SCENE_DATA_SPACE1			(8)												//场景数据空间1大小
#define SCENE_DATA_SPACE2			(16)											//场景数据空间2大小
#define SCENE_DATA_SPACE3			(32)											//场景数据空间3大小

#define SCENE_SPACE1_NUM			(128)											//场景空间1的总数
#define SCENE_SPACE2_NUM			(64)											//场景空间2的总数
#define SCENE_SPACE3_NUM			(64)											//场景空间3的总数

#if (SCENCETIME_EN > 0u)
/*场景延时参数EEPROM内存分配,注意延时时间内存块的标号从1开始,最多支持56个*/
#define SCENE_TIME_DATA_START_ADD	(SCENE_DATA_START_ADD + SCENE_DATA_SPACE)		//场景时间数据存储地址(起始地址0x1C80)
#define SCENE_TIME_SPACE			(0x500)											//场景时间数据占用空间大小
#define SCENE_TIME_TOTAL_NUM		(SCENE_TIME_SPACE1_NUM +\
									 SCENE_TIME_SPACE2_NUM +\
									 SCENE_TIME_SPACE3_NUM)							//场景时间场景数量最大数

#define SCENE_TIME_DATA_SPACE1      (10)											//场景时间空间1的大小
#define SCENE_TIME_DATA_SPACE2      (35)											//场景时间空间2的大小
#define SCENE_TIME_DATA_SPACE3      (50)											//场景时间空间3的大小

#define SCENE_TIME_SPACE1_NUM		(32)											//场景延时空间1的数量
#define SCENE_TIME_SPACE2_NUM		(16)											//场景延时空间2的数量
#define SCENE_TIME_SPACE3_NUM		(8)												//场景延时空间3的数量
#endif

/*联动数据存储地址*/
#define ALINK_START_ADDR            (0x2400)										//最简联动首地址
#define ALINK_SPACE                 (0x0700)                                		//最简联动空间

/*联动时间数据存储地址*/
#define ALINK_TIME_START_ADDR       (0x2B00)										//延时时间首地址
#define ALINK_TIME_SPACE            (0x0200)										//延时时间空间

/*数据类型定义*/
#pragma pack(1)     				//按1字节对齐
typedef struct						//系统共用数据区域
{
	u32 userID;						//用户号
	u32 deviceID;					//设备ID
	u8 	sendCnt;					//数据发送次数
	u8  serialNum;  				//流水号
	u32 BandAddr[5];				//转发绑定地址
	u8  BandAreaEn;					//转发绑定区域广播指令使能标志
}SysData_t;							//系统共用数据区

typedef struct 						//单元数据区域
{
	u8 	area[4];					//区域号
	u16 type;						//所属类型
	u8 	able;						//单元使能标志位
	u8  common;						//单元共用大类
	u8  UploadAble; 				//上报使能标志
	u8  UploadSetTime;				//上报最大随机时间
}UnitData_t;

typedef union
{
	struct
	{
		u16 sceneNum: 8;		//场景号
		u16 area	: 3;		//区域号
		u16 unit	: 5;		//单元号
	}scene;
	u16 flag;
}SceneInfo_t;

typedef struct 
{
	SceneInfo_t SceneInfo;	
#if (SCENCETIME_EN > 0u)	
	u8 sceneTimeSavePos;		//场景存储位置,1~56
#endif
}SceneIndexes_t;

typedef struct
{
	SceneIndexes_t SceneIndexes[SCENE_MAX_NUM];//场景的存储地址,0~255
	u8 sceneUseFlag[SCENE_USE_SPACE];
#if (SCENCETIME_EN > 0u)
	u8 sceneTimeUseFlag[SCENE_TIME_USE_SPACE];
#endif
}sceneManage_t;

#pragma pack()
typedef enum 
{
	FristLevel = 1,
	SecondLevel = 2,
}DataInitLevel_t;


extern SysData_t gSysData;

void UserData_Init(void);
void UserEEPROMData_Init(DataInitLevel_t level);

/*设备渠道信息数据操作*/
u32 GetDeviceChannelFlag(void);
u8 SetDeviceChannelFlag(const u32 deviceUseNum,const u8 operateLevel);

/*异常统计EEPROM数据操作*/
void FaultData_Init(void);
void FaultData_Save(u8 i);
void FaultData_Get(u8 *len, u8 *data);

/*系统EEPROM数据操作*/
void SysEEPROMData_Init(void);
u8 SysEEPROMData_Set(u8 size , u8 *data); 

/*单元共用EEPROM数据操作*/
void UnitPublicEEPROMData_Init(void);
void UnitPublicEEPROMUnitData_Init(u8 unit);
void UnitPublicEEPROMData_Get(u8 unitCnt, UnitData_t *unitData);
u8 UnitPublicEEPROMData_Set(u8 size , u8 *data);

/*单元共用EEPROM数据操作*/
void UnitPrivateEEPROMData_Init(void);
void UnitPrivateEEPROMUnitData_Init(u8 unit);

/*组EEPROM数据操作*/
void GroupEEPROM_Init(void);
void GroupEEPROMUnit_Init(const u8 unit);
void GroupEEPROM_Clr(const u8 unit, const  u8 area, const  u8 num);
void GroupEEPROM_Set(const u8 unit,const u8 area,const u8 num);
u8 GroupEEPROM_Get(const u8 unit,const u8 area,const u8 num);

/*场景EEPROM数据操作*/
void SceneData_Init(void);
void SceneEEPROM_Init(void);
void SceneEEPROMUnit_Init(u8 unit);
void SceneEEPROM_Clr(const u8 unit, const u8 area, const u16 num);
void SceneEEPROM_Get(u8 unit, u8 area, u8 num, u8 *data);
u32 SceneEEPROM_Set(u8 unit, u8 area, u8 num, u8 *data);

#if (SCENCETIME_EN > 0u)
u32 SceneEEPROM_SetEx(u8 unit, u8 area, u8 num, u8 *data);
u8 SceneEEPROM_GetEx(u8 unit, u8 area, u8 num, u8 *data);
void CheckSceneTimeData(u8 unit, u8 area, u8 num);
void CheckSceneDlyTimeEEPROM(void);
#endif

#endif // UserData.h end 
/**************************Copyright BestFu 2014-05-14*************************/
