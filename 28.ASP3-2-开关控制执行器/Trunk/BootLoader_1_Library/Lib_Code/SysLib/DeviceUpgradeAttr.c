/***************************Copyright BestFu 2014-05-14*************************
文	件：	FlaseAttr.c
说	明：	关于ICFlash数据操作相关属性
编	译：	Keil uVision4 V4.54.0.0
版	本：	v1.0
编	写：	Unarty
日	期：	2013-11-22
修  改:     2014.12.25 Unarty 依据不同存储大小的Flash,重新分区程序代码运行空间大小
*******************************************************************************/
#include "lib_type.h"
#include "lib_config.h"
#include "PivotalDataType.h"
#include "Updata.h"
#include "Thread.h"
#include "si4432.h"
//#include "sys.h"
#include "iwdg.h"
#include "DevPartTab.h"
#include "DeviceUpgradeAttr.h"

//define private variable
static MsgResult_t Set_IAPDataCRC_Attr(UnitPara_t *pData);
static MsgResult_t Get_IAPPackCheck_Attr(UnitPara_t *pData, BF_INT08U *rlen, BF_INT08U *rpara);
static MsgResult_t Get_DevUpgradeReady_Attr(UnitPara_t *pData, BF_INT08U *rlen, BF_INT08U *rpara);

/*******************************************************************************
函 数 名： 	Set_IAPReady_Attr
功能说明：  设置升级准备属性
参	  数:   data[0]: 目标单元
			data[1]: 参数长度
返 回 值：  消息执行结果
*******************************************************************************/
MsgResult_t Set_IAPReady_Attr(UnitPara_t *pData)
{
    IWDG_Feed();
	switch(pData->data[0])
	{
	case 0xF1:
	{
		return (MsgResult_t)UpgradePreStage(pData);
	}
	case 0xF3:
	{
		return Set_IAPDataCRC_Attr(pData);
	}
	default:
	{
		return PARA_MEANING_ERR;
	}     
	}
}

/*******************************************************************************
函 数 名:  	Get_IAPReady_Attr
功能说明:  	获取设备升级准备状态
参    数:  	data[0]: 目标单元
			data[1]: 参数长度
			rLen:	返回参数长度
			rpara:	返回参数内容
返 回 值:  	消息执行结果
*******************************************************************************/
MsgResult_t Get_IAPReady_Attr(UnitPara_t *pData, BF_INT08U *rlen, BF_INT08U *rpara)
{
    IWDG_Feed();
    switch(pData->data[0])
    {
    case 0xF0:
    {
        return (MsgResult_t)Get_DevUpgradeReady_Attr(pData, rlen, rpara);
    }
    case 0xF2:
    {
        return (MsgResult_t)Get_IAPPackCheck_Attr(pData, rlen, rpara);
    }
    default:
	{
		return PARA_MEANING_ERR;
	}   
    }
}

/*******************************************************************************
函 数 名： 	Get_DevUpgradeReady_Attr
功能说明：  获取设备升级准备状态
参	  数:   data[0]: 目标单元
			data[1]: 参数长度
			rLen:	返回参数长度
			rpara:	返回参数内容
返 回 值:  	消息执行结果
*******************************************************************************/
static MsgResult_t Get_DevUpgradeReady_Attr(UnitPara_t *pData, BF_INT08U *rlen, BF_INT08U *rpara)
{
    DevPivotalSet_t* pDevPivotalSet = GetDevPivotalSet();
    Soft_t* pSoft = (Soft_t*)(pDevPivotalSet->DevSoftInfoAddr);
    Hard_t* pHard = (Hard_t*)(pDevPivotalSet->DevHardInfoAddr);

    if(UPGRADE_APP == pData->data[1])  				//升级app
    {
        *rlen = 11;
        *(BF_INT16U*)&rpara[0] = pSoft->deviceType;
        *(BF_INT32U*)&rpara[2] = pHard->hardVer;
        *(BF_INT32U*)&rpara[6] = pSoft->SoftVer;
        *(BF_INT08U*)&rpara[10]= pHard->chipType;
        return COMPLETE;
    }
    else if(UPGRADE_BOOTLOADER == pData->data[1])  	//升级bootloader
    {
        return BOOT_UPGRADE_AIM_ERR;				//引导程序不能升级引导程序
    }
    return PARA_MEANING_ERR;
}


/*******************************************************************************
函 数 名:  	Get_IAPPackCheck_Attr
功能说明:  	获取设备属性
参    数:  	data[0]: 目标单元
			data[1]: 参数长度
			rLen:	返回参数长度
			rpara:	返回参数内容
返 回 值:  	消息执行结果
*******************************************************************************/
static MsgResult_t Get_IAPPackCheck_Attr(UnitPara_t *pData, BF_INT08U *rlen, BF_INT08U *rpara)
{
    if(UpgradeSrc == pData->msgsrc)
	{
		if (0xF1 == UPDATA_LEVEL)
		{
			if (Updata_PackCheck(rlen, (BF_INT16U*)rpara))  	//没有掉包
			{
				UPDATA_LEVEL = 0xF2;
			}
			return COMPLETE;
		}
		else if (0xF2 == UPDATA_LEVEL)
		{
			return COMPLETE;
		}
		else
		{
			UPDATA_LEVEL = 0;
			return UPGRADE_STATE_ERR;
		}
	}
	return NO_ACK;
}

/*******************************************************************************
函 数 名:  	Set_IAPDataCRC_Attr
功能说明:  	获取升级数据校验结果
参    数:  	data[0]: 目标单元
			data[1]: 参数长度
			rLen:	返回参数长度
			rpara:	返回参数内容
返 回 值:  	消息执行结果
*******************************************************************************/
static MsgResult_t Set_IAPDataCRC_Attr(UnitPara_t *pData)
{
	if(UpgradeSrc == pData->msgsrc)
	{
		if (0xF2  == UPDATA_LEVEL)
		{
			//交换两个数的值
			*(BF_INT08U*)&pData->data[5] ^= *(BF_INT08U*)&pData->data[6];
			*(BF_INT08U*)&pData->data[6] ^= *(BF_INT08U*)&pData->data[5];
			*(BF_INT08U*)&pData->data[5] ^= *(BF_INT08U*)&pData->data[6];

			if (Updata_Calibrate(*(BF_INT32U*)&pData->data[1], *(BF_INT16U*)&pData->data[5]) == TRUE)
			{
				if (Updata_Flag())  	//更新升级标记
				{
					UPDATA_LEVEL = 0xF3;
					IWDG_FeedOFF();
					Thread_Login(ONCEDELAY,1,800,SystemReset);
	//				JumpToCode(DevPartTab->user_app_start_addr);
					return COMPLETE;
				}
			}
		}
		else if (0xF3 == UPDATA_LEVEL)
		{
			return COMPLETE;
		}
		else
		{
			UPDATA_LEVEL = 0;
			return UPGRADE_STATE_ERR;
		}
	}
	return NO_ACK;
}


/*******************************************************************************
函 数 名： 	Set_IAPPackSave_Attr
功能说明：  升级数据包保存
参	  数:   data[0]: 目标单元
			data[1]: 参数长度
返 回 值： 	消息执行结果
*******************************************************************************/
MsgResult_t Set_IAPPackSave_Attr(UnitPara_t *pData)
{
	IWDG_Feed();
	if(UpgradeSrc == pData->msgsrc)
	{
		if (0xF1 == UPDATA_LEVEL)  	//判断升级状态是否到达F1
		{
			return (MsgResult_t)Updata_PackProcess(*(BF_INT16U*)&pData->data[0],\
													   pData->len - 2, (BF_INT08U*)&pData->data[2]);//数据包处理
		}
		else if (0xF2 == UPDATA_LEVEL)
		{
			return COMPLETE;
		}
		else
		{
			UPDATA_LEVEL = 0;
			return UPGRADE_STATE_ERR;
		}
	}
	return NO_ACK;
}

/*******************************************************************************
函 数 名： 	Set_433Channel_Attr
功能说明：  设置433信道属性
参	  数:   data[0]: 目标单元
			data[1]: 参数长度
返 回 值：  消息执行结果
*******************************************************************************/
MsgResult_t Set_433Channel_Attr(UnitPara_t *pData)
{
    if ((pData->len != 1))  	//参数区长度不合法
    {
        return PARA_LEN_ERR;
    }
    else if (pData->data[0] < CHANNEL_MIN     //参数区内容不合法
             || pData->data[0] > CHANNEL_MAX)
    {
        return PARA_MEANING_ERR;
    }

    if (pData->data[0]  != Channel_Get())
    {
        if (Updata_Channel(pData->data[0]) == FALSE)  	//更新信道失败
        {
            return CMD_EXE_ERR;
        }
        Thread_Login(ONCEDELAY, 0, 800, &Si4432_Init);	//关闭喂狗，设备自动进入重启
    }
    return COMPLETE;
}

/*******************************************************************************
函 数 名:  	Get_433Channel_Attr
功能说明:  	获取433信道属性
参    数:  	data[0]: 目标单元
			data[1]: 参数长度
			rLen:	返回参数长度
			rpara:	返回参数内容
返 回 值:  	消息执行结果
*******************************************************************************/
MsgResult_t Get_433Channel_Attr(UnitPara_t *pData, BF_INT08U *rlen, BF_INT08U *rpara)
{
    *rpara = Channel_Get();		//信道值
    *rlen = 1;

    return COMPLETE;
}

/**************************Copyright BestFu 2014-05-14*************************/
