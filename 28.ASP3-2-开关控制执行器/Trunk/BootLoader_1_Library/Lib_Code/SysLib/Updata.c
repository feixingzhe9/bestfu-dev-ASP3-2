/***************************Copyright BestFu 2016-04-28*************************
文	件：	Updata.c
说	明：	升级数据处理函数
编	译：	uVision V5.12.0
版	本：	v1.0
编	写：	jay
日	期：	2016-04-28
修　改：	无
*******************************************************************************/
#include "lib_type.h"
#include "lib_config.h"
#include "crc.h"
#include "sys.h"
#include "BestFuLib.h"
#include "EEPROM.h"
#include "PivotalDataType.h"
#include "Updata.h"
#include "DevPartTab.h"
#include "program.h"

//Private variable define

//Globe variable define
Soft_t IAP_FLAG;    	//IAP标志
RadioPara_t RADIO_PARA ; //433通信参数
UpgradePre_t UpgradePre = {&IAP_FLAG};
BF_INT08U UPDATA_LEVEL = 0xEF; 				//标记升级标记，更新完成级别 F0/F1/F2/F3/F4
BF_INT08U UpgradeSrc = RF_433M_INTERFACE;	//默认升级源为433

//Private function  declarartiuon
static BF_BOOLEAN UpgradeReady(UpgradePre_t* pUpgradePre);

/*******************************************************************************
函 数 名：  void UpgradePreStage(UnitPara_t *pData)
功能说明：  升级准备F0阶段
参	  数：  data[0]: 操作子命令
			data[1]: 升级对象
			data[2~3]: 设备类型
			data[4~7]: 升级文件的总大小
			data[8]: 每个升级包的大小
			data[9~12]: 升级软件的版本号
返 回 值：  返回升级状态
*******************************************************************************/
BF_INT32U UpgradePreStage(UnitPara_t *pData)
{
    BF_INT32U result = 0u;

    if(UPGRADE_APP == pData->data[1]) 		//检测升级目标,是否是升级APP程序
    {
        UpgradeSrc 			   			= pData->msgsrc;		//记录目标升级源
		UpgradePre.pSoft->deviceType 	= *(BF_INT16U*)(&pData->data[2]);
        UpgradePre.pSoft->appSize 		= *(BF_INT32U*)(&pData->data[4]);
        UpgradePre.pSoft->SoftVer 		= *(BF_INT32U*)(&pData->data[9]);
        UpgradePre.packetsize	  		= *(BF_INT08U*)(&pData->data[8]);
        UpgradePre.packetnum      		= NUM_ROUND(UpgradePre.pSoft->appSize,UpgradePre.packetsize);

        if(TRUE == (result = UpgradeReady(&UpgradePre)))
        {
            UPDATA_LEVEL = 0xF1;
            return COMPLETE;
        }
        return (result);
    }
    return BOOT_UPGRADE_AIM_ERR;
}


/*******************************************************************************
函 数 名：  UpgradeReady
功能说明：  升级准备
参	  数：  pUpgradePre：升级准备的存储指针
返 回 值：  准备结果 TRUE(完成）/FALSE(失败）
*******************************************************************************/
static BF_INT08U UpgradeReady(UpgradePre_t* pUpgradePre)
{
    BF_INT32U userSize = 0u;	//防止更新程序空间越界

    if (pUpgradePre->packetsize & 0x03)  	//如果包的大小不是按4字节对齐
    {
        return PACKET_SIZE_ALIGN_ERR;	//包大小字节对齐错误
    }

    userSize = DevPartTab->user_data_start_addr - DevPartTab->user_app_start_addr;

    if (pUpgradePre->pSoft->appSize > userSize)  	//如果应用空间大于可用空间大小
    {
        return APP_SIZE_BOUND_ERR;	//升级包大小越界
    }
    memset(pUpgradePre->data,0,sizeof(UpgradePre_t)-offsetof(UpgradePre_t,data));
    return Program_Erase(DevPartTab->user_app_start_addr, pUpgradePre->pSoft->appSize);  //对应存储空间清零
}

/*******************************************************************************
函 数 名：  Updata_PackProcess
功能说明：  升级数据包处理
参	  数：  id:		对应包的ID
			len:	包内数据长度
			data:	数据内容
返 回 值：  FALSE/TRUE
*******************************************************************************/
BF_BOOLEAN Updata_PackProcess(BF_INT32U id, BF_INT08U len, BF_INT08U *data)
{
    BF_INT32U addr;

    if ((id > (UpgradePre.packetnum - 1))|| (len > 200))
    {
        return FALSE;
    }
    if (!(UpgradePre.data[id >> 5]&(1<<(id & 0x1f))))
    {

        addr = id*UpgradePre.packetsize + DevPartTab->user_app_start_addr;
        if (Program_Write(addr, len, data))
        {

            UpgradePre.data[id >> 5] |= (1<<(id & 0x1f)); //接收包ID置1
        }
        else
        {
            return FALSE;
        }
    }
    return TRUE;
}

/*******************************************************************************
函 数 名：	Updata_PackCheck
功能说明： 	升级数据包核对
参	  数： 	*len：	缺包数据长度
			*data:  缺包ID
返 回 值：	TRUE/FLASE
*******************************************************************************/
BF_BOOLEAN Updata_PackCheck(BF_INT08U *num, BF_INT16U *data)
{
    BF_INT32U i;

    for (i = 0, *num = 0; i < UpgradePre.packetnum; i++)
    {
        if (!(UpgradePre.data[i >> 5]&(1 << (i & 0x1f))))   //包未收到
        {
            *data++ = i;
            (*num) += 2;
            if ((*num) > 100)
            {
                break;
            }
        }
    }
    return (*num == 0 ? TRUE : FALSE);
}

/*******************************************************************************
函 数 名：	Updata_Calibrate
功能说明： 	设备升级结果校验
参	  数： 	len:	数据长度
			crc:	校验码
返 回 值：	TRUE/FLASE
*******************************************************************************/
BF_INT08U Updata_Calibrate(BF_INT32U len, BF_INT16U crc)
{
    crc -= ChkCrcValue((BF_INT08U*)DevPartTab->user_app_start_addr, len);
    return ((crc) ? FALSE : TRUE);
}

/*******************************************************************************
函 数 名：	Updata_Flag
功能说明： 	升级标识
参	  数： 	add:	标识区存储地址
返 回 值：	TRUE/FLASE
*******************************************************************************/
BF_INT08U Updata_Flag(void)
{
    DevPivotalSet_t* pDevPivotalSet = GetDevPivotalSet();
    BF_INT08U ch = Channel_Get();

    IAP_FLAG.jumpFlag = Flash_DefaultValue();
    Program_Erase(pDevPivotalSet->DevSoftInfoAddr,DevPartTab->user_app_start_addr - pDevPivotalSet->DevSoftInfoAddr);	//修改Flag区域
    Program_Write(pDevPivotalSet->DevSoftInfoAddr, sizeof(Soft_t), (BF_INT08U*)&IAP_FLAG);

    return Updata_Channel(ch);
}

/*******************************************************************************
函 数 名：	Updata_Channel
功能说明： 	更新433通道
参	  数： 	ch:		修改后的信道值
返 回 值：	TRUE/FLASE
*******************************************************************************/
BF_INT08U Updata_Channel(BF_INT08U ch)
{
    DevPivotalSet_t* pDevPivotalSet = GetDevPivotalSet();
    RadioPara_t* pRadioPara = (RadioPara_t*)(pDevPivotalSet->DevRadioParaAddr);
    RADIO_PARA.channel = (BF_INT32U)ch ;

    for (; (BF_INT32U)pRadioPara < DevPartTab->user_app_start_addr; pRadioPara++)
    {
        if (*(BF_INT32U*)pRadioPara == Flash_DefaultValue())  	//数据没有修改
        {
            return Program_Write((BF_INT32U)pRadioPara, offsetof(RadioPara_t,Reserve), (BF_INT08U*)&RADIO_PARA); //返回修改结果
        }
        Program_Read((BF_INT32U)pRadioPara + offsetof(RadioPara_t,Reserve),\
                     sizeof(RadioPara_t) - offsetof(RadioPara_t,Reserve),\
                     (BF_INT08U*)RADIO_PARA.Reserve);
    }
    Program_Read(pDevPivotalSet->DevSoftInfoAddr, sizeof(Soft_t), (BF_INT08U*)&IAP_FLAG);
    Program_Erase(pDevPivotalSet->DevSoftInfoAddr, DevPartTab->user_app_start_addr - pDevPivotalSet->DevSoftInfoAddr);	//清空数据
    Program_Write(pDevPivotalSet->DevSoftInfoAddr, sizeof(Soft_t), (BF_INT08U*)&IAP_FLAG);	//数据写入

    return Updata_Channel(ch);
}

/*******************************************************************************
函 数 名：	Channel_Get
功能说明： 	获取433通道值
参	  数： 	无
返 回 值：	433通道值
*******************************************************************************/
BF_INT08U Channel_Get(void)
{
    DevPivotalSet_t* pDevPivotalSet = GetDevPivotalSet();
    RadioPara_t* pRadioPara = (RadioPara_t*)(pDevPivotalSet->DevRadioParaAddr);

    for (; (BF_INT32U)pRadioPara < DevPartTab->user_app_start_addr; pRadioPara ++)
    {
        if (*(BF_INT32U*)pRadioPara == Flash_DefaultValue())  	//数据没有修改
        {
            break;
        }
    }
    -- pRadioPara;	//指针指向有效空间

    Program_Read((BF_INT32U)pRadioPara,sizeof(RadioPara_t),(BF_INT08U*)(&RADIO_PARA));
    if ((pRadioPara->channel >= CHANNEL_MIN) &&\
            (pRadioPara->channel <= CHANNEL_MAX))
    {
        return (pRadioPara->channel);	//返回最后通道设置值
    }
    return (RADIO_PARA.channel = 0x53);		//返回默认通道值
}

/*******************************************************************************
函 数 名：	AppJump_Init
功能说明： 	bootloader1跳转初始化
参	  数： 	无
返 回 值：	返回跳转状态
*******************************************************************************/
void AppJump_Init(void)
{
    DevPivotalSet_t* pDevPivotalSet = GetDevPivotalSet();
    Soft_t* pSoft = (Soft_t*)(pDevPivotalSet->DevSoftInfoAddr);

    if(BOOTLOADER_1_JUMP_FLAG != pSoft->jumpFlag)
    {
        JumpToCode(DevPartTab->user_app_start_addr);
    }
}
/**************************Copyright BestFu 2015-04-28*************************/
