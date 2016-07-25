/***************************Copyright BestFu 2016-04-16*************************
文	件：	DevPartTab.h
说	明：	设备分区表处理函数头文件
编	译：	uVision V5.12.0
版	本：	v1.0
编	写：	jay
日	期：	2016-04-16
修  改:     暂无
*******************************************************************************/
#ifndef __DEV_PARTITION_TABEL_H
#define __DEV_PARTITION_TABEL_H

/*********************************自定义宏*************************************/
#define DEVICE_PARTION_TABLE_ADDR	((u32)(0x08000380))		//设备分区表的起始地址
	
/********************************数据结构定义**********************************/ 
typedef struct		//设备分区表
{
	u32 boot0_start_addr;			//bootloader0的起始地址
	u32 boot0_flag_start_addr;		//bootloader0_flag的起始地址
	u32 boot1_start_addr;			//bootloader1的起始地址
	u32 boot1_flag_start_addr;		//bootloader1_flag的起始地址
	u32 user_app_start_addr;		//用户app的起始地址
	u32 user_data_start_addr;		//用户数据区的保留地址
	u32 flash_end_addr;				//用户flash的结束地址
}DevPartionTable_t;

typedef struct						//设备关键信息集合
{
	u32 DevHardInfoAddr;			//设备硬件信息地址
	u32 Boot0FlagAddr;				//bootloader0标志区信息地址
	u32 DevResetParaAddr;			//设备重设地址信息地址
	u32 DevFlashEncryptAddr;		//设备flash加密信息地址
	u32 DevSoftInfoAddr;			//设别软件信息地址
	u32 DevRadioParaAddr;			//433通讯相关信息地址
}DevPivotalSet_t;

#define DevPartTab			((DevPartionTable_t*)(DEVICE_PARTION_TABLE_ADDR))	//设备分区表的地址信息

/********************************外部变量声明**********************************/
extern const DevPivotalSet_t DevPivotalSet[3];		//设备关键信息分区表信息

/**********************************函数声明***********************************/  
void DevPivotalSetInit(void);
DevPivotalSet_t* GetDevPivotalSet(void);

#endif
/**************************Copyright BestFu 2016-04-16*************************/
