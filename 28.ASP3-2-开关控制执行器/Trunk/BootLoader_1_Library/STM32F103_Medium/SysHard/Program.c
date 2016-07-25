/***************************Copyright BestFu 2014-05-14*************************
文	件：    Program.c
说	明：    STM32_Flash烧入
编	译：    Keil uVision4 V4.54.0.0
版	本：    v1.0
编	写：    Unarty
日	期：    2013-11-21
修　改：	暂无
*******************************************************************************/
#include "lib_type.h"
#include "lib_config.h"
#include "stm32f10x.h"
#include "flash.h"
#include "UserData.h"
#include "DevPartTab.h"
#include "program.h"
#include "DeviceIDSetAttr.h"

//Private macro define
#define FLASH_DEFAULT		(0xFFFFFFFF)		//默认FLASH的初值
#define PAGE_SIZE     		(0x400)				//页地址大小
#define STM32_FLASH_BASE    (0x08000000)		//STM32 FLASH基地址

/*******************************************************************************
函 数 名：	Program_Erase
功能说明： 	擦除要编程的位置内容
参	  数： 	addr:   写入数据的首地址
            size:   要写入数据的大小
返 回 值：	擦除结果TRUE/FALSE
*******************************************************************************/
BF_BOOLEAN Program_Erase(BF_INT32U addr, BF_INT32U size)
{   
	Flash_Unlock();
    while (FLASH_ErasePage(addr)&&(size > PAGE_SIZE))
    {
        size -= PAGE_SIZE;
        addr += PAGE_SIZE;
    }
  	Flash_lock();

    return (size > PAGE_SIZE ? FALSE : TRUE);
}

/*******************************************************************************
函 数 名：	Program_Write
功能说明： 	写入编程
参	  数： 	addr:    写入数据的首地址
            size:   要写入数据的大小
            data:   写入数据内容
返 回 值：	写入结果TRUE/FALSE
*******************************************************************************/
BF_BOOLEAN Program_Write(BF_INT32U addr, BF_INT32U len, BF_INT08U *data)
{
	BF_INT08U flag;
	
	if (len & 0x01)		//内部为半字节写入
	{
		len++;
	}
	Flash_Unlock();
	flag = Flash_Write(addr, len, data);
	Flash_lock();

	return flag;
}

/*******************************************************************************
函 数 名：	Program_Read
功能说明： 	读出编程数据内容
参	  数： 	addr:   读出数据的首地址
            size:   要读出数据的大小
            data:   读出数据内容
返 回 值：	写入结果TRUE/FALSE
*******************************************************************************/
BF_BOOLEAN Program_Read(BF_INT32U addr, BF_INT32U len, BF_INT08U *data)
{
    len = (len&0x03)?(((len&(~0x03))|(1u<<2))):(len);
	while (len > 0)
    {
        *(volatile BF_INT32U*)data = *(volatile BF_INT32U*)addr;
        data += 4;
        addr += 4;
        len  -= 4;
    }
    
	return TRUE;
}
  
#if (FLASH_ENCRYPTION_EN > 0u)
/*******************************************************************************
函 数 名：	void FlashEncryptionInit(void)
功能说明： 	Flash加密初始化
参	  数： 	无
返 回 值：	无
*******************************************************************************/
void FlashEncryptionInit(void)
{
	BF_INT32U backp_tab[18];
	BF_INT16U encryptflag_pos= 0u;
	
	RCC->AHBENR |= RCC_AHBENR_CRCEN;
	CRC->CR		|= CRC_CR_RESET;
	CRC->IDR	|= CRC_IDR_IDR;
	if(ENCRYPTION_FLAG != *(volatile BF_INT32U*)(ENCRYPTION_FLAG_ADDR))
	{
		encryptflag_pos=(GetDevPivotalSet())->DevFlashEncryptAddr - DevPartTab->boot0_flag_start_addr;
		backp_tab[encryptflag_pos]		= ENCRYPTION_FLAG;
		backp_tab[encryptflag_pos + 1]	= FlashEncryptionFunc(*(volatile BF_INT32U*)(RESET_DEV_ID_FLAG_ADDR + 4));
		if(*(volatile BF_INT32U*)(ENCRYPTION_FLAG_ADDR) == FLASH_DEFAULT)
		{
			Program_Write(ENCRYPTION_FLAG_ADDR,sizeof(FlashEncrypt_t),(BF_INT08U*)(&backp_tab[encryptflag_pos]));
		}
		else
		{
			Program_Read(DevPartTab->boot0_flag_start_addr,encryptflag_pos + sizeof(FlashEncrypt_t),(BF_INT08U*)&backp_tab);
			Program_Erase(DevPartTab->boot0_flag_start_addr,DevPartTab->boot1_start_addr - DevPartTab->boot0_flag_start_addr);		
			Program_Write(DevPartTab->boot0_flag_start_addr,encryptflag_pos + sizeof(FlashEncrypt_t),(BF_INT08U *)&backp_tab);	
		}	
	}
}
#endif

#if (DEVICE_ADDR_RESET_FEATURE_EN > 0u)
/*******************************************************************************
函 数 名：	void Get_UniqueDeviceID(u8* chipidtable)
功能说明： 	获取stm32 flash全球唯一ID
参	  数：  chipidtable:存储96位全球唯一ID的首地址
返 回 值：	无
*******************************************************************************/
void Get_UniqueDeviceID(BF_INT08U* chipidtable)
{
	*(volatile BF_INT32U*)(&chipidtable[0]) = *((volatile BF_INT32U*)(CHIPID_ADDR + 0x00));
	*(volatile BF_INT32U*)(&chipidtable[4]) = *((volatile BF_INT32U*)(CHIPID_ADDR + 0x04));
	*(volatile BF_INT32U*)(&chipidtable[8]) = *((volatile BF_INT32U*)(CHIPID_ADDR + 0x08));
}
#endif

/*******************************************************************************
函 数 名：	Flash_DefaultValue
功能说明： 	False默认值
参	  数： 	void
返 回 值：	默认状态下Flashw值
*******************************************************************************/
BF_INT32U Flash_DefaultValue(void)
{
	return (FLASH_DEFAULT);
}

/*******************************************************************************
函 数 名：	Flash_PageSize
功能说明： 	False扇区大小
参	  数： 	void
返 回 值：	False扇区大小
*******************************************************************************/
BF_INT32U Flash_PageSize(void)
{
	return (PAGE_SIZE);
}

/*******************************************************************************
函 数 名：	Flash_ChipSize
功能说明： 	False芯片大小
参	  数： 	void
返 回 值：	False芯片大小
*******************************************************************************/
BF_INT32U Flash_ChipSize(void)
{
	return (*(volatile BF_INT16U*)(USER_FLASH_AREA));
}

/**************************Copyright BestFu 2014-05-14*************************/
