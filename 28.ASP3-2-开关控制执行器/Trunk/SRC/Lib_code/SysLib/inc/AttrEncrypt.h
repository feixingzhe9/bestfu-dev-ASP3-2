/***************************Copyright BestFu 2014-05-14*************************
文	件：	AttrEncrypt.h
说	明：	属性加密头文件
编	译：	Keil uVision4 V4.54.0.0
版	本：	v1.0
编	写：	Unarty
日	期：	2015-01-26
修　改：	暂无
*******************************************************************************/
#ifndef __ATTRENCRYPT_H
#define __ATTRENCRYPT_H

#include "BF_type.h"
#include "Command.h"

typedef struct 
{
	u32 srcAddr;	//源地址
	u8  time;		//等待时间
	u8  cnt;
	u16 crc;		//
	u16 prv;		//私有密钥
	u16 prd;		//公用密钥
}Encrypt_t;

u8 AttrEncrypt_Check(Communication_t * cmd);	//属性加密核对
void Encrypt_FlagSet(void);

#endif  //Updata.h  end
/**************************Copyright BestFu 2014-05-14*************************/
