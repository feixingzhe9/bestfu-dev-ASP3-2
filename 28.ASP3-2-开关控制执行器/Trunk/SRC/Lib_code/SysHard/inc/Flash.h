/***************************Copyright BestFu 2014-05-14*************************
文	件：	Flash.h
说	明：	STM32F103_Flash操作头文件
编	译：	Keil uVision4 V4.54.0.0
版	本：	v1.0
编	写：	Unarty
日	期：	2013-11-21
修  改:     暂无
*******************************************************************************/
#ifndef __FLASH_H
#define __FLASH_H

#include "BF_type.h"

/*********************************配置功能*************************************/
#define FLASH_ENCRYPTION_EN				(1u)				//使能flash加密功能
#define DEVICE_ADDR_RESET_FEATURE_EN	(1u)				//使能地址重设功能

#define DEVICE_ID			 	(*(vu32*)(0x1ffff7f0))		//设备ID取自IC全球唯一ID   F103
#define CHIPID_ADDR			 	(0X1FFFF7E8)    			//STM32F103CX全球96位唯一芯片ID存储首地址
#define USER_FLASH_AREA		 	(0x1FFFF7E0)				//F103芯片大小

#if ((FLASH_ENCRYPTION_EN > 0u)||(DEVICE_ADDR_RESET_FEATURE_EN > 0u))
#define RE_DEVICE_START_ADDR	(0x08000430)				//设备重设地址数据起始地址	
#endif

#if (FLASH_ENCRYPTION_EN > 0u)
/*****************************Flash加密相关函数********************************/
#define CRC_BASE_DR				((u32)(0x40023000))			//CRC_DR寄存器的首地址
#define CRC_BASE_CR				((u32)(0x40023008))			//CRC_CR寄存器的首地址
#define ENCRYPTION_START_ADDR	((u32)(0x0800043C))			//设备flash加密数据的首地址	

__inline u32 FlashEncryptionFunc(u32 NewAddr)				//计算加密值使用
{
	*((vu32*)(CRC_BASE_CR))|= (u32)(0x01);
	*((vu32*)(CRC_BASE_DR)) = *((vu32*)(CHIPID_ADDR + 0x00));
	*((vu32*)(CRC_BASE_DR)) = *((vu32*)(CHIPID_ADDR + 0x04));
	*((vu32*)(CRC_BASE_DR)) = *((vu32*)(CHIPID_ADDR + 0x08));
	*((vu32*)(CRC_BASE_DR)) = NewAddr;
	return (*((vu32*)(CRC_BASE_DR)));
}
__inline u32 DecryptionFlash(void)				//使用加密值在函数中混淆使用
{
	*((vu32*)(CRC_BASE_CR))|=  (u32)(0x01);
	*((vu32*)(CRC_BASE_DR)) = *((vu32*)(CHIPID_ADDR + 0x00));
	*((vu32*)(CRC_BASE_DR)) = *((vu32*)(CHIPID_ADDR + 0x04));
	*((vu32*)(CRC_BASE_DR)) = *((vu32*)(CHIPID_ADDR + 0x08));
	*((vu32*)(CRC_BASE_DR)) = *(vu32*)(RE_DEVICE_START_ADDR + 4);
	*((vu32*)(CRC_BASE_DR)) = *(vu32*)(ENCRYPTION_START_ADDR + 4);
	return (*((vu32*)(CRC_BASE_DR)));
}
#endif

/*******************************数据类型定义***********************************/
typedef enum
{
    BUSY     = 0x01,
    PGERR    = 0x04,
    WRITEERR = 0x10,
    EOP      = 0x20
}Flash_SR;

/*******************************函数声明**************************************/
void Flash_Unlock(void);
void Flash_lock(void);
u8 FLASH_ErasePage(u32 page_add);
u8 Flash_ProgramHalfWord(u32 add, u16 data);
u8 Flash_Write(u32 add, u32 len, u8 *data);
u8 Flash_WriteProtect(u32 addr, u8 sta);
u8 Flash_EraseOptionByte(void);
u8 Flash_WriteOptionByte(u32 addr, u8 data);

#endif /*FLASH_H */

/**************************Copyright BestFu 2014-05-14*************************/
