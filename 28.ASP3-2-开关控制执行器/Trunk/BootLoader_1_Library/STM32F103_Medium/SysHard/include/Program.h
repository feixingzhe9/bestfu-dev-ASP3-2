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
/**********************************自定义宏************************************/

/**********************************函数声明************************************/
BF_BOOLEAN Program_Erase(BF_INT32U addr, BF_INT32U size);
BF_BOOLEAN Program_Write(BF_INT32U addr, BF_INT32U len, BF_INT08U *data);
BF_BOOLEAN Program_Read(BF_INT32U addr, BF_INT32U len, BF_INT08U *data) ;
#if (FLASH_ENCRYPTION_EN > 0u)
void FlashEncryptionInit(void);
#endif
#if (DEVICE_ADDR_RESET_FEATURE_EN > 0u)
void Get_UniqueDeviceID(BF_INT08U* chipidtable);
#endif
BF_INT32U Flash_DefaultValue(void);
BF_INT32U Flash_PageSize(void);
BF_INT32U Flash_ChipSize(void);

#endif //program.h
/**************************Copyright BestFu 2014-05-14*************************/
