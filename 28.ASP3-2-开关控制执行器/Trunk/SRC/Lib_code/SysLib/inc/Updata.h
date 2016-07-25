/***************************Copyright BestFu 2014-05-14*************************
文	件：	Updata.h
说	明：	升级数据处理函数
编	译：	Keil uVision4 V4.54.0.0
版	本：	v1.0
编	写：	Unarty
日	期：	2013-11-24
修　改：	暂无
*******************************************************************************/
#ifndef __UPDATA_H
#define __UPDATA_H

#include "BF_type.h"
#include "UnitCfg.h"

#define CHANNEL_MIN				(73)
#define CHANNEL_MAX				(93)

#define BOOTLOADER_0_JUMP_FLAG	(0xAA788755)		//标记bootloader0的跳转方向
#define BOOTLOADER_1_JUMP_FLAG	(0x5AC33CA5)		//标记bootloader1跳转标记

enum								//定义升级的目标
{
	UPGRADE_APP 		= 0x01,		//升级用户APP程序
	UPGRADE_BOOTLOADER	= 0x02		//升级设备的引导程序
};

typedef struct
{
	u32 hardVer;			//硬件版本
	u32 Reserve1;			//保留
	u32 chipType; 			//芯片类型
	u32 channelFlag[4];		//渠道标识
	u32 data;				//日期
	u32 time;				//时间
	u8  str[100];
}Hard_t;

typedef  struct
{
	u32 jumpFlag;			//Bootloader_1程序跳转标记 
    u32 deviceType;			//设备类型
    u32	SoftVer;			//软件版本号
    u32	appSize;			//APP程序的大小，以字节为单位  
}Soft_t;

typedef struct
{
	u32 jumpFlag;			//Bootloader_0跳转标记
	u32 SoftVer;			//bootloader1软件版本
	u32 appSize;			//bootloader1 APP大小
}Boot0Info_t;

#pragma pack(1)
typedef  struct
{
    u32 channel; 			//信道
	u32 Reserve[3];			//保留空间
}RadioPara_t;
#pragma pack()

typedef  struct
{
	Boot0Info_t* pBootInfo;	//指向软件信息的指针
	u16 packetnum;			//升级包的数量
	u8 packetsize;   		//升级包大小
	u32 data[64];			//已传数据包的大小
}UpgradePre_t;

extern u8 UPDATA_LEVEL; 	//记录升级状态
extern u8 UpgradeAim;		//记录升级目标

u32 UpgradePreStage(UnitPara_t *pData);
u8 Updata_PackProcess(u32 id, u8 len, u8 *data);
u8 Updata_PackCheck(u8 *num, u16 *data);
u8 Updata_Calibrate(u32 len, u16 crc);
u8 Updata_Flag(void);
u8 Updata_Channel(u8 ch);
u8 Channel_Get(void);
u8 Updata_433EncryptFlag(u8 flag);
u8 Get_433EncryptFlag(void);

#endif  //Updata.h  end
/**************************Copyright BestFu 2014-05-14*************************/
