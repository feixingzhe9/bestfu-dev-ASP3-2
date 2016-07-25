/***************************Copyright BestFu 2014-05-14*************************
文	件：	Upload.h
说	明：	设备主动上报相关函数头文件
编	译：	Keil uVision4 V4.54.0.0
版	本：	v1.0
编	写：	Unarty
日	期：	2014.07.03 
修　改：	暂无
*******************************************************************************/
#ifndef __UPLOAD_H
#define __UPLOAD_H

#include "BF_type.h"

#define MAX_UPLOAD_UNIT  (24)  //最大上报单元数量  yanhuan adding 2016/01/08

typedef struct
{
	u32 cnt ;
	u32 time ;
	u8	flag ;	
} UploadCondition_t;

typedef enum
{
	RESTART			= 0,	//上电/重启
	USER_DATA_RESET	= 1,	//用户数据重置/恢复出厂设置
	EEPROM_FAIL 	= 2,	//用户数据存储区域异常	
	WIFI_433		= 3, 	//433无线通信失败
}Upload_t;

//函数声明
void Upload_Fault(Upload_t upload);
void Upload_Immediately(u8 unit,u8 cmd,u8* data);
void SysTickUpload(void);
#endif //Upload.h end
/**************************Copyright BestFu 2014-05-14*************************/
