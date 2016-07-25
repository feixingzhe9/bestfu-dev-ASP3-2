/***************************Copyright BestFu 2014-05-14*************************
文	件：	DeviceIDSetAttr.h
说	明：	重设设备地址的头文件
编	译：	uVision V5.12.0.0
版	本：	v1.0
编	写：	Unarty
日	期：	2013-11-22
修  改:     暂无
*******************************************************************************/
#ifndef __DEVICE_ID_SET_H 
#define __DEVICE_ID_SET_H
#include <UnitCfg.h>

/**********************************用户自定义宏********************************/
//重设地址部分
#define RE_DEVICE_ID_FLAG						((u32)(0xA5C33C5A))			//标记重设地址标记
//判断重设地址的有效性
#define DEVICE_ID_IS_OK(flag,addr,anti_addr)	(((RE_DEVICE_ID_FLAG == (flag))&&\
												(0xffffffff != (addr))&&\
												(0x00000000 != (addr))&&\
												(0x44444444 != (addr))&&\
												(0x88888888 != (addr))&&\
												(0x54534542 != (addr))&&\
												((addr) == ~(anti_addr))))
//FLAH加密部分
#define ENCRYPTION_FLAG							((u32)(0x5ABD42A5))			//标记FLASH加密标记

/**********************************数据结构定义********************************/
typedef struct
{
	u32 SetAddrFlag;			//重设有效设置地址标记
	u32 DeviceID;				//重设的设备地址
	u32 DeviceIDInver;			//重设地址的反码
}DeviceIDSet_t;

typedef struct
{
	u32 EncryptFlashFlag;		//FLASH加密标记
	u32 EncryptFlashData;		//Flash加密密钥
}FlashEncrypt_t;

/**********************************函数声明************************************/
MsgResult_t Get_ResetAddr_Attr(UnitPara_t *pData, u8 *rlen, u8 *rpara);
MsgResult_t Set_ResetAddr_Attr(UnitPara_t *pData);

#endif //DeviceIDSetAttr.h
/**************************Copyright BestFu 2014-05-14*************************/
