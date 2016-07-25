/***************************Copyright BestFu 2014-05-14*************************
文	件：	Flash.c
说	明：	Flash操作函数
编	译：	Keil uVision4 V4.54.0.0
版	本：	v1.0
编	写：	Unarty
日	期：	2013-11-21
修  改:     暂无
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
函 数 名：	Flash_Unlock
功能说明： 	STM32_Flash解锁
参	  数： 	无
返 回 值：	无
*******************************************************************************/
void Flash_Unlock(void)
{
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;
}

/*******************************************************************************
函 数 名：	Flash_Unlock
功能说明： 	STM32_Flash上锁
参	  数： 	无
返 回 值：	无
*******************************************************************************/
void Flash_lock(void)
{
    FLASH->CR |= FLASH_CR_LOCK;
}

/*******************************************************************************
函 数 名：	FLASH_ErasePage
功能说明： 	Flash页擦除
参	  数： 	page_add:  业地址
返 回 值：  TRUE/FALSE
*******************************************************************************/
uint8_t FLASH_ErasePage(uint32_t page_add)
{
    uint8_t flag = Flash_Status(EraseTime);

    if (!flag)//没有flash错误
    {
        FLASH->CR |= FLASH_CR_PER; //使能页擦除
        FLASH->AR = page_add;//要擦除页地址
        FLASH->CR |= FLASH_CR_STRT;//开启一次页擦除 
        
        flag = Flash_Status(EraseTime);
        FLASH->CR &= ~FLASH_CR_PER;   //清除页擦除
    }
    if (!flag)
    {
        flag  = Flash_EOP();
    }
    
    return flag;
}

/*******************************************************************************
函 数 名：	Flash_Status
功能说明： 	获取Flash状态寄存器值
参	  数：  timeOut:    检测超时时间
返 回 值：	Flash_status
*******************************************************************************/
static uint8_t Flash_Status(uint32_t timeOut)
{
    for (;(timeOut > 0)&&(FLASH->SR&FLASH_SR_BSY); timeOut--)//等待Flash空闲
    {}
    if (!timeOut)//等待超时
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
函 数 名：	Flash_ProgramHalfWord
功能说明： 	写入半个字节
参	  数：  add:    写入地址
            data:   写入数据内容
返 回 值：	TRUE/FLASE
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
函 数 名：	Flash_Write
功能说明： 	flash写入数据
参	  数：  add:    写入首地址
            len:    写入数据大小
            data:   写入数据内容
返 回 值：	TRUE/FLASE
*******************************************************************************/
uint8_t Flash_Write(uint32_t add, uint32_t len, uint8_t *data)
{
    if (!Flash_Status(ProgramTime))
    {
        FLASH->CR |= FLASH_CR_PG;  //进入编程
        while (len > 0)
        {
            *(__IO uint16_t*)add = *(uint16_t*)data;
            data += 2;
            add  += 2;
            len  -= 2;
            if (!Flash_Status(ProgramTime))//未超时
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
函 数 名：	Flash_EOP
功能说明： 	获取Flash操作结果标志位
参	  数：  无
返 回 值：	FALSE/TRUE (Flash写入或擦除操作结果)
*******************************************************************************/
static uint8_t Flash_EOP(void)
{
    if (FLASH->SR&FLASH_SR_EOP)//操作成功
    {
        FLASH->SR = FLASH_SR_EOP; //清除标志
        return 1;
    }

    return 0;
}

/*******************************************************************************
函 数 名：	Flash_eraseOptionByte
功能说明： 	擦除整个Flash选项字节
参	  数：  无
返 回 值：	TRUE/FALSE (操作结果)
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
函 数 名：	Flash_WriteOptionByte
功能说明： 	写入Flash选项字节
参	  数：  addr:   选项字节地址
            data:   内容
返 回 值：	TRUE/FALSE (操作结果)
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


