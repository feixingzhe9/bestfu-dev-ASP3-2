/***************************Copyright BestFu 2014-05-14*************************
文	件：    Program.h
说	明：    STM32_Flash烧入
编	译：    Keil uVision4 V4.54.0.0
版	本：    v1.0
编	写：    Unarty
日	期：    2013-11-21
修　改：	暂无
*******************************************************************************/
#ifndef __PROGRAM_H
#define __PROGRAM_H

#include "BF_type.h"
#include "flash.h"

#define PAGE_SIZE   (1024) //1Kb

/**********************************函数声明***********************************/ 
u8 Program_Erase(u32 addr, u32 size);
u8 Program_Read(u32 addr, u32 len, u8 *data);
u8 Program_Write(u32 addr, u32 len, u8 *data);
u32 Flash_DefaultValue(void);
u32 Flash_PageSize(void);
u32 Flash_ChipSize(void);
u8 Program_WriteEx(u32 addr, u32 len, u8 *data);
#if (FLASH_ENCRYPTION_EN > 0u)
void FlashEncryptionInit(void);
#endif

#if (DEVICE_ADDR_RESET_FEATURE_EN > 0u)
void Get_UniqueDeviceID(u8* chipidtable);
#endif

#endif //program.h
/**************************Copyright BestFu 2014-05-14*************************/
