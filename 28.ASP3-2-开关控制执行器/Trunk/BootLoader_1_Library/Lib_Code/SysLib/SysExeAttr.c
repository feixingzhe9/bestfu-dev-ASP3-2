/***************************Copyright BestFu 2014-05-14*************************
文	件：    SysExeAttr.c
说	明：    系统级属性命令执行表，所有命令的实现
编	译：    Keil uVision4 V4.54.0.0
版	本：    v2.0
编	写：    Unarty
日	期：    2014.06.26
修　改：	李诺淮 2014.10.22 添加无线包测试接口 B0～BF
			Unarty 2014.12.01 删除0xCE 与0xCF 属性，（此接口是为兼容V1.00.00版本而预留的，现已完全去除此接口）
*******************************************************************************/
#include "lib_type.h"
#include "lib_config.h"
#include "PivotalDataType.h"
#include "Updata.h"
#include "Thread.h"
#include "DataEncrypt.h"
#include "UserData.h"
#include "DeviceUpgradeAttr.h"
#include "DevPartTab.h"
#include "FaultManage.h"
#include "Upload.h"
#include "UnitCfg.h"
#include "UnitShare_Attr.h"
#include "SysExeAttr.h"


/*属性读写列表*/
const AttrExe_st SysAttrTable[] =
{
    {0xC0, LEVEL_0, NULL							, Get_DeviceType_Attr				},
    {0xC1, LEVEL_0, NULL							, Get_Version_Attr					},
    {0xC9, LEVEL_0, Set_UserDataFormat_Attr			, NULL								},
    {0xF4, LEVEL_0, Set_IAPReady_Attr				, Get_IAPReady_Attr					},
    {0xF5, LEVEL_0, Set_IAPPackSave_Attr			, NULL								},
    {0xF8, LEVEL_0, Set_433Channel_Attr				, Get_433Channel_Attr				},
};

/*******************************************************************************
函 数 名:  Sys_UnitAttrNum
功能说明:  计算系统单元属性数量
参    数:  无
返 回 值:  无
*******************************************************************************/
BF_INT08U Sys_UnitAttrNum(void)
{
    return (BF_DIM(SysAttrTable));
}

/*******************************************************************************
函 数 名:  Upload_RESTART
功能说明:  设备上报重启
参    数:  无
返 回 值:  无
*******************************************************************************/
static void Upload_RESTART(void)
{
    Upload_Fault(RESTART);	//上报设备重启
}

/*******************************************************************************
函 数 名:  SysAttr_Init
功能说明:  系统属性初始化
参    数:  unitID:	单元号
返 回 值:  无
*******************************************************************************/
void SysAttr_Init(BF_INT08U unitID)
{
    UserData_Init();
    Fault_Upload(FAULT_2 , DEV_RESTART , NULL); //设备重启次数记录保存     yanhuan adding 2015/10/10
    Thread_Login(ONCEDELAY , 0 ,1000 , Upload_RESTART); //1秒之后上报重启  yanhuan adding 2015/12/02
    EncryptInit((BF_INT08U*)&gSysData.deviceID); //433信号加密初始化
}

/*******************************************************************************
函 数 名：  Set_UserDataFormat_Attr
功能说明：  设置用户数据恢复出厂设置
参	  数：  data[0]: 目标单元
			data[1]: 参数长度
返 回 值:  	消息执行结果
*******************************************************************************/
MsgResult_t Set_UserDataFormat_Attr(UnitPara_t *pData)
{
    if((1 == pData->len) && (0 == pData->data[0]))   //恢复出厂设置 需要恢复433信道
    {
        UserEEPROMData_Init(SecondLevel);
    }
    else if (0 == pData->len)  						//数据长度为零  默认处理单元为0
    {
        UserEEPROMData_Init(FristLevel);
    }
    else if (pData->data[0] < UnitCnt_Get())     	//参数正确
    {
        UnitPublicEEPROMUnitData_Init(pData->data[0]);
        UnitPrivateEEPROMUnitData_Init(pData->data[0]);
    }
    else
    {
        return PARA_MEANING_ERR;
    }

    return COMPLETE;
}

/*******************************************************************************
函 数 名:  	Get_Version_Attr
功能说明:  	获取版本信息
参    数:  	data[0]: 目标单元
			data[1]: 参数长度
			rLen:	返回参数长度
			rpara:	返回参数内容
返 回 值:  	消息执行结果
*******************************************************************************/
MsgResult_t Get_Version_Attr(UnitPara_t *pData, BF_INT08U *rlen, BF_INT08U *rpara)
{
    BF_INT08U *p = NULL;
    DevPivotalSet_t* pDevPivotalSet = GetDevPivotalSet();
    Soft_t* pSoft = (Soft_t*)(pDevPivotalSet->DevSoftInfoAddr);
    Hard_t* pHard = (Hard_t*)(pDevPivotalSet->DevHardInfoAddr);
    Boot0Info_t* pBoot0Info = (Boot0Info_t*)(pDevPivotalSet->Boot0FlagAddr);

    *rlen = 0;
    *(BF_INT16U*)&rpara[*rlen] = 0xffff;				//系统库版本
    *rlen += 2;
    *(BF_INT32U*)&rpara[*rlen] = ((BOOTLOADER_1_JUMP_FLAG == pSoft->jumpFlag)?(0xFFFFFFFF):(pSoft->SoftVer));		//软件版本
    *rlen += sizeof(pSoft->SoftVer);
    *(BF_INT32U*)&rpara[*rlen] = pBoot0Info->SoftVer;
    *rlen += sizeof(pBoot0Info->SoftVer);
    *(BF_INT32U*)&rpara[*rlen] = pHard->hardVer;		//硬件版本
    *rlen += sizeof(pHard->hardVer);

    for (p = pHard->str; (*p <= 'Z')&&(*p >= '0'); p++)  	//硬件描述信息
    {
        rpara[((*rlen)++)] = *p;
    }

    return COMPLETE;
}

/*******************************************************************************
函 数 名:  	Get_DeviceType_Attr
功能说明:  	获取设备类型
参    数:  	data[0]: 目标单元
			data[1]: 参数长度
			rLen:	返回参数长度
			rpara:	返回参数内容
返 回 值:  	消息执行结果
*******************************************************************************/
MsgResult_t Get_DeviceType_Attr(UnitPara_t *pData, BF_INT08U *rlen, BF_INT08U *rpara)
{
    DevPivotalSet_t* pDevPivotalSet = GetDevPivotalSet();
    Soft_t* pSoft = (Soft_t*)(pDevPivotalSet->DevSoftInfoAddr);
    if (!(pData->len)	//全局查找
            || (pSoft->deviceType == *(BF_INT16U*)&pData->data[0])	//按设备类型查找
            || (0xFFFF == *(BF_INT16U*)&pData->data[0]))  			//全类型查找
    {
        *rlen = 0;
        *(BF_INT16U*)&rpara[*rlen] = pSoft->deviceType;
        *rlen += 2;
        *(BF_INT32U*)&rpara[*rlen] = pSoft->SoftVer;
        *rlen += 4;

        return COMPLETE;
    }

    return CMD_EXE_ERR;
}


/**************************Copyright BestFu 2014-05-14*************************/
