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

/*******************************�û��Զ����***********************************/
#define DEVICE_ID			 (*(__IO uint32_t*)(0x1ffff7f0))	//�豸IDȡ��ICȫ��ΨһID   F103
#define CHIPID_ADDR			 (0X1FFFF7E8)    					//STM32F103CXȫ��96λΨһоƬID�洢�׵�ַ
#define USER_FLASH_AREA		 (0x1FFFF7E0)						//оƬ�Ĵ�С

#if ((FLASH_ENCRYPTION_EN > 0u)||(DEVICE_ADDR_RESET_FEATURE_EN > 0u))
#define RESET_DEV_ID_FLAG_ADDR	((uint32_t)(0x08000430))		//��¼�豸�����ַ��ǵ���Ϣ��ַ

#endif
								
#if (FLASH_ENCRYPTION_EN > 0u)
///*****************************Flash������غ���********************************/
#define CRC_BASE_DR				((uint32_t)(0x40023000))							//CRC_DR�Ĵ������׵�ַ
#define CRC_BASE_CR				((uint32_t)(0x40023008))							//CRC_CR�Ĵ������׵�ַ
#define ENCRYPTION_FLAG_ADDR	((uint32_t)(0x0800043c))							//��¼FLASH���ܵı�ʶ��Ϣ��ַ

__inline uint32_t FlashEncryptionFunc(uint32_t NewAddr)	//�������ֵʹ��
{
	*((__IO uint32_t*)(CRC_BASE_CR))|= (uint32_t)(0x01);
	*((__IO uint32_t*)(CRC_BASE_DR)) = *((__IO uint32_t*)(CHIPID_ADDR + 0x00));
	*((__IO uint32_t*)(CRC_BASE_DR)) = *((__IO uint32_t*)(CHIPID_ADDR + 0x04));
	*((__IO uint32_t*)(CRC_BASE_DR)) = *((__IO uint32_t*)(CHIPID_ADDR + 0x08));
	*((__IO uint32_t*)(CRC_BASE_DR)) = NewAddr;
	return (*((__IO uint32_t*)(CRC_BASE_DR)));
}
__inline uint32_t DecryptionFlash(void)				//ʹ�ü���ֵ�ں����л���ʹ��
{
	*((__IO uint32_t*)(CRC_BASE_CR))|=  (uint32_t)(0x01);
	*((__IO uint32_t*)(CRC_BASE_DR)) = *((__IO uint32_t*)(CHIPID_ADDR + 0x00));
	*((__IO uint32_t*)(CRC_BASE_DR)) = *((__IO uint32_t*)(CHIPID_ADDR + 0x04));
	*((__IO uint32_t*)(CRC_BASE_DR)) = *((__IO uint32_t*)(CHIPID_ADDR + 0x08));
	*((__IO uint32_t*)(CRC_BASE_DR)) = *(__IO uint32_t*)(RESET_DEV_ID_FLAG_ADDR + 4);
	*((__IO uint32_t*)(CRC_BASE_DR)) = *(__IO uint32_t*)(ENCRYPTION_FLAG_ADDR + 4);
	return (*((__IO uint32_t*)(CRC_BASE_DR)));
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
uint8_t FLASH_ErasePage(uint32_t page_add);
uint8_t Flash_ProgramHalfWord(uint32_t add, uint16_t data);
uint8_t Flash_Write(uint32_t add, uint32_t len, uint8_t *data);
uint8_t Flash_EraseOptionByte(void);
uint8_t Flash_WriteOptionByte(uint32_t addr, uint8_t data);

#endif /*FLASH_H */

/**************************Copyright BestFu 2014-05-14*************************/
