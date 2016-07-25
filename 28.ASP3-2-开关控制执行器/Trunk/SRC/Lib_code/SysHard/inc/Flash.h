/***************************Copyright BestFu 2014-05-14*************************
��	����	Flash.h
˵	����	STM32F103_Flash����ͷ�ļ�
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2013-11-21
��  ��:     ����
*******************************************************************************/
#ifndef __FLASH_H
#define __FLASH_H

#include "BF_type.h"

/*********************************���ù���*************************************/
#define FLASH_ENCRYPTION_EN				(1u)				//ʹ��flash���ܹ���
#define DEVICE_ADDR_RESET_FEATURE_EN	(1u)				//ʹ�ܵ�ַ���蹦��

#define DEVICE_ID			 	(*(vu32*)(0x1ffff7f0))		//�豸IDȡ��ICȫ��ΨһID   F103
#define CHIPID_ADDR			 	(0X1FFFF7E8)    			//STM32F103CXȫ��96λΨһоƬID�洢�׵�ַ
#define USER_FLASH_AREA		 	(0x1FFFF7E0)				//F103оƬ��С

#if ((FLASH_ENCRYPTION_EN > 0u)||(DEVICE_ADDR_RESET_FEATURE_EN > 0u))
#define RE_DEVICE_START_ADDR	(0x08000430)				//�豸�����ַ������ʼ��ַ	
#endif

#if (FLASH_ENCRYPTION_EN > 0u)
/*****************************Flash������غ���********************************/
#define CRC_BASE_DR				((u32)(0x40023000))			//CRC_DR�Ĵ������׵�ַ
#define CRC_BASE_CR				((u32)(0x40023008))			//CRC_CR�Ĵ������׵�ַ
#define ENCRYPTION_START_ADDR	((u32)(0x0800043C))			//�豸flash�������ݵ��׵�ַ	

__inline u32 FlashEncryptionFunc(u32 NewAddr)				//�������ֵʹ��
{
	*((vu32*)(CRC_BASE_CR))|= (u32)(0x01);
	*((vu32*)(CRC_BASE_DR)) = *((vu32*)(CHIPID_ADDR + 0x00));
	*((vu32*)(CRC_BASE_DR)) = *((vu32*)(CHIPID_ADDR + 0x04));
	*((vu32*)(CRC_BASE_DR)) = *((vu32*)(CHIPID_ADDR + 0x08));
	*((vu32*)(CRC_BASE_DR)) = NewAddr;
	return (*((vu32*)(CRC_BASE_DR)));
}
__inline u32 DecryptionFlash(void)				//ʹ�ü���ֵ�ں����л���ʹ��
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

/*******************************�������Ͷ���***********************************/
typedef enum
{
    BUSY     = 0x01,
    PGERR    = 0x04,
    WRITEERR = 0x10,
    EOP      = 0x20
}Flash_SR;

/*******************************��������**************************************/
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
