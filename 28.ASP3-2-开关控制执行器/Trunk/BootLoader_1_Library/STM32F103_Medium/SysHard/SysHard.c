/***************************Copyright BestFu 2014-05-14*************************
文	件：    SysHard.c
说	明：    系统硬件相关函数
编	译：    Keil uVision4 V4.54.0.0
版	本：    v2.0
编	写：    Unarty
日	期：    2014.06.26
修　改：	暂无
*******************************************************************************/
#include "lib_type.h"
#include "lib_config.h"
#include "stm32f10x.h"
#include "i2c.h"
#include "SysTick.h"
#include "flash.h"
#include "SI4432.h"
#include "userdata.h"
#include "PivotalDataType.h"
#include "DeviceIDSetAttr.h"
#include "DevPartTab.h"
#include "program.h"
#include "usart.h"
#include "DevPartTab.h"
#include "SysHard.h"
#include "iwdg.h"

/*******************************************************************************
函 数 名：	SysHard_Init
功能说明： 	初始化系统硬件相关内容
参	  数： 	无
返 回 值：	无
*******************************************************************************/
void SysHard_Init(void)
{
    SysTick_Init();				//初始化系统时钟
#if (FLASH_ENCRYPTION_EN > 0u)
    FlashEncryptionInit();
#endif
    I2C_Init();		//IIC初始化
#if (RF_433M_INTERFACE_EN > 0u)
    Si4432_Init();				//433无线模块初始化
#endif
#if (USART_INTERFACE_EN > 0u )
    USART_Init(9600);
#endif
    IWDG_Config(5,2000);
}

/*******************************************************************************
函 数 名：	HardID_Check
功能说明： 	硬件地址核对
参	  数： 	id: 传入匹配ID
返 回 值：	0（地址匹配成功)/否则返回设备真实ID
*******************************************************************************/
BF_INT32U HardID_Check(BF_INT32U id)
{
#if (DEVICE_ADDR_RESET_FEATURE_EN > 0u)
    DevPivotalSet_t* pDevPivotalSet = GetDevPivotalSet();
    DeviceIDSet_t* pDeviceIDSet = (DeviceIDSet_t*)(pDevPivotalSet->DevResetParaAddr);

    if(DEVICE_ID_IS_OK(pDeviceIDSet->SetAddrFlag,\
                       pDeviceIDSet->DeviceID,pDeviceIDSet->DeviceIDInver)) 		//如果已设置的新地址合法
    {
        if(pDeviceIDSet->DeviceID != id)
        {
            return (pDeviceIDSet->DeviceID);
        }
    }
    else
#endif
    {
        if (DEVICE_ID != id)
        {
            return DEVICE_ID;
        }
    }//End of if(DEVICE_ID_IS_OK)
    return 0;
}

/**************************Copyright BestFu 2014-05-14*************************/
