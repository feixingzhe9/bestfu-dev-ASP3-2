/***************************Copyright BestFu 2014-05-14*************************
��	����	Flash.c
˵	����	Flash��������
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2013-11-21
��  ��:     ����
*******************************************************************************/
#include "lib_type.h"
#include "lib_config.h"
#include "stm32f10x.h"
#include "DeviceIDSetAttr.h"
#include "DevPartTab.h"
#include "flash.h"

/* FLASH Keys */
#define FLASH_KEY1				((uint32_t)0x45670123)
#define FLASH_KEY2				((uint32_t)0xCDEF89AB)

#define EraseTime 				((uint32_t)0x2000)
#define ProgramTime				((uint32_t)0x3000)

static uint8_t Flash_EOP(void);
static uint8_t Flash_Status(uint32_t timeOut);

/*******************************************************************************
�� �� ����	Flash_Unlock
����˵���� 	STM32_Flash����
��	  ���� 	��
�� �� ֵ��	��
*******************************************************************************/
void Flash_Unlock(void)
{
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;
}

/*******************************************************************************
�� �� ����	Flash_Unlock
����˵���� 	STM32_Flash����
��	  ���� 	��
�� �� ֵ��	��
*******************************************************************************/
void Flash_lock(void)
{
    FLASH->CR |= FLASH_CR_LOCK;
}

/*******************************************************************************
�� �� ����	FLASH_ErasePage
����˵���� 	Flashҳ����
��	  ���� 	page_add:  ҵ��ַ
�� �� ֵ��  TRUE/FALSE
*******************************************************************************/
uint8_t FLASH_ErasePage(uint32_t page_add)
{
    uint8_t flag = Flash_Status(EraseTime);

    if (!flag)//û��flash����
    {
        FLASH->CR |= FLASH_CR_PER; //ʹ��ҳ����
        FLASH->AR = page_add;//Ҫ����ҳ��ַ
        FLASH->CR |= FLASH_CR_STRT;//����һ��ҳ���� 
        
        flag = Flash_Status(EraseTime);
        FLASH->CR &= ~FLASH_CR_PER;   //���ҳ����
    }
    if (!flag)
    {
        flag  = Flash_EOP();
    }
    
    return flag;
}

/*******************************************************************************
�� �� ����	Flash_Status
����˵���� 	��ȡFlash״̬�Ĵ���ֵ
��	  ����  timeOut:    ��ⳬʱʱ��
�� �� ֵ��	Flash_status
*******************************************************************************/
static uint8_t Flash_Status(uint32_t timeOut)
{
    for (;(timeOut > 0)&&(FLASH->SR&FLASH_SR_BSY); timeOut--)//�ȴ�Flash����
    {}
    if (!timeOut)//�ȴ���ʱ
    {
        return BUSY;
    }
    if (FLASH->SR&(FLASH_SR_PGERR|FLASH_SR_WRPRTERR))
	{
		FLASH->SR = FLASH_SR_PGERR|FLASH_SR_WRPRTERR;
		return 1;
	}

    return 0;
}

/*******************************************************************************
�� �� ����	Flash_ProgramHalfWord
����˵���� 	д�����ֽ�
��	  ����  add:    д���ַ
            data:   д����������
�� �� ֵ��	TRUE/FLASE
*******************************************************************************/
uint8_t Flash_ProgramHalfWord(uint32_t add, uint16_t data)
{
    uint8_t flag = Flash_Status(ProgramTime);

    if (!flag)
    {
        FLASH->CR |= FLASH_CR_PG;
        *(__IO uint16_t*)add = (uint16_t)data;
        flag = Flash_Status(ProgramTime);
    
        FLASH->CR &= ~FLASH_CR_PG;
    }
    if (!flag)
    {
        if (Flash_EOP())
        {
            flag = 1; 
        }
    }

    return flag;
}

/*******************************************************************************
�� �� ����	Flash_Write
����˵���� 	flashд������
��	  ����  add:    д���׵�ַ
            len:    д�����ݴ�С
            data:   д����������
�� �� ֵ��	TRUE/FLASE
*******************************************************************************/
uint8_t Flash_Write(uint32_t add, uint32_t len, uint8_t *data)
{
    if (!Flash_Status(ProgramTime))
    {
        FLASH->CR |= FLASH_CR_PG;  //������
        while (len > 0)
        {
            *(__IO uint16_t*)add = *(uint16_t*)data;
            data += 2;
            add  += 2;
            len  -= 2;
            if (!Flash_Status(ProgramTime))//δ��ʱ
            {
                if (!Flash_EOP())
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }
    
        FLASH->CR &= ~FLASH_CR_PG;
    } 

    return (len == 0 ? 1 : 0);
}

/*******************************************************************************
�� �� ����	Flash_EOP
����˵���� 	��ȡFlash���������־λ
��	  ����  ��
�� �� ֵ��	FALSE/TRUE (Flashд�������������)
*******************************************************************************/
static uint8_t Flash_EOP(void)
{
    if (FLASH->SR&FLASH_SR_EOP)//�����ɹ�
    {
        FLASH->SR = FLASH_SR_EOP; //�����־
        return 1;
    }

    return 0;
}

/*******************************************************************************
�� �� ����	Flash_eraseOptionByte
����˵���� 	��������Flashѡ���ֽ�
��	  ����  ��
�� �� ֵ��	TRUE/FALSE (�������)
*******************************************************************************/
uint8_t Flash_EraseOptionByte(void)
{
    uint8_t flag = 0;
    
    if (!Flash_Status(ProgramTime))
    {
        Flash_Unlock();
        FLASH->CR |= FLASH_CR_OPTER;
        FLASH->CR |= FLASH_CR_STRT;
        if (!Flash_Status(ProgramTime))
        {
            if (Flash_EOP())
            {
                flag = 1;
            }
        }
          FLASH->CR &= ~FLASH_CR_OPTER;
        
    }
    return flag;
}


/*******************************************************************************
�� �� ����	Flash_WriteOptionByte
����˵���� 	д��Flashѡ���ֽ�
��	  ����  addr:   ѡ���ֽڵ�ַ
            data:   ����
�� �� ֵ��	TRUE/FALSE (�������)
*******************************************************************************/
uint8_t Flash_WriteOptionByte(uint32_t addr, uint8_t data)
{
    uint8_t flag = 0;
    
    if (!Flash_Status(ProgramTime))
    {
        Flash_Unlock();
        FLASH->CR |= FLASH_CR_OPTPG;
        FLASH->CR |= FLASH_CR_STRT;
        *(__IO uint8_t*)addr = data;
        if (!Flash_Status(ProgramTime))
        {
            if (Flash_EOP())
            {
                flag = 1;
            }
        }
        FLASH->CR &= ~FLASH_CR_OPTER;    
    }
    return flag;
}
/**************************Copyright BestFu 2014-05-14*************************/


