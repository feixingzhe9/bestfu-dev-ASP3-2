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

/*********************************自定义宏*************************************/
#define CHANNEL_MIN				(73)
#define CHANNEL_MAX				(93)

#define BOOTLOADER_0_JUMP_FLAG	(0xAA788755)		//标记bootloader0的跳转方向
#define BOOTLOADER_1_JUMP_FLAG	(0x5AC33CA5)		//标记bootloader1跳转标记
/********************************数据结构定义**********************************/   
enum								//定义升级的目标
{
	UPGRADE_APP 		= 0x01,		//升级用户APP程序
	UPGRADE_BOOTLOADER	= 0x02		//升级设备的引导程序
};

typedef struct
{
	BF_INT32U hardVer;				//硬件版本
    BF_INT32U Reserve1;				//保留
    BF_INT32U chipType;				//芯片类型
	BF_INT32U channelFlag[4];		//渠道标识
    BF_INT32U data;					//日期
    BF_INT32U time;					//时间
    BF_INT08U str[100];
}Hard_t;

typedef  struct
{
	BF_INT32U jumpFlag;				//Bootloader_1程序跳转标记
	BF_INT32U deviceType;			//设备类型
    BF_INT32U SoftVer;				//软件版本号
    BF_INT32U appSize;				//APP程序的大小，以字节为单位 
}Soft_t;

typedef struct
{
	BF_INT32U jumpFlag;				//Bootloader_0程序跳转标记
	BF_INT32U SoftVer;				//bootloader1软件版本
	BF_INT32U appSize;				//bootloader1软甲大小
}Boot0Info_t;

typedef  struct
{
    BF_INT32U channel; 				//信道
	BF_INT32U Reserve[3];			//保留空间
}RadioPara_t;

#pragma pack(1)
typedef  struct
{
	Soft_t*   pSoft;				//指向软件信息的指针
	BF_INT16U packetnum;			//升级包的数量
	BF_INT08U packetsize;   		//升级包大小
	BF_INT32U data[64];				//已传数据包的大小
}UpgradePre_t;
#pragma pack()

/***********************************************************************/
extern BF_INT08U UPDATA_LEVEL; 	//标记升级标记
extern BF_INT08U UpgradeSrc;	//消息来源标记

/**********************************函数声明***********************************/ 
BF_INT32U UpgradePreStage(UnitPara_t *pData);
BF_INT08U Updata_Channel(BF_INT08U ch);
BF_INT08U Channel_Get(void);
BF_BOOLEAN Updata_PackCheck(BF_INT08U *num, BF_INT16U *data);
BF_INT08U Updata_Calibrate(BF_INT32U len, BF_INT16U crc);
BF_BOOLEAN Updata_PackProcess(BF_INT32U id, BF_INT08U len, BF_INT08U *data);
BF_INT08U Updata_Flag(void);
void AppJump_Init(void);

#endif  //Updata.h  end
/**************************Copyright BestFu 2014-05-14*************************/
