/***************************Copyright BestFu 2014-05-14*************************
文 件：  UserData.c
说 明：  用户数据管理文件
编 译：  Keil uVision4 V4.54.0.0
版 本：  v1.0
编 写：  Unarty
日 期：  2014.06.24
修　改：    2014.11.11 Uarty 修改UserData_Init中的初值判断，
            减少因EEPROM异常出现用户数据丢失的可能性。
*******************************************************************************/
#include "lib_type.h"
#include "lib_config.h"
#include "EEPROM.h"
#include "Thread.h"
#include "SysHard.h"
#include "Upload.h"
#include "UserData.h"
#include "BestFuLib.h"
#include "PivotalDataType.h"
#include "FaultManage.h"
#include "UnitCfg.h"
#include "DeviceUpgradeAttr.h"
#include "UserData.h"

//Globe variable define
SysData_t gSysData;
/*******************************************************************************
函 数 名：  UserData_Init()
功能说明：  用户数据初始化
参   数：   无
返 回 值：  无
修   改：   Uarty 2014.11.11 增加Cnt，减少因EEPROM异常而导致数据删除。
*******************************************************************************/
void UserData_Init(void)
{
    static BF_INT08U cnt = 0;  // Unarty Add 2014.11.11

    ReadDataFromEEPROM(SYSDATA_START_ADD, sizeof(gSysData), (BF_INT08U *)&gSysData);   //获取初次使用标识
    if (HardID_Check(gSysData.deviceID) != 0)   								//设备ID不相同
    {
        Thread_Login(ONCEDELAY, 0, 50, UserData_Init);      					//反复验证，防止EEPROM上电时异常// Unarty Add 2014.11.11
        if (++cnt > 3)   // Unarty Add 2014.11.11
        {
            Thread_Logout(UserData_Init);       								//去除返复验证接口// Unarty Add 2014.11.11
            FaultData_Init();
            UserEEPROMData_Init(FristLevel);        							//初始化用户EEPROM数据
        }
    }
    else if (cnt > 0)
    {
        cnt = 0;            													//Unarty 2014.12.02 修改，防止死锁。
        Unit_Init();
    }
}

/*******************************************************************************
函 数 名：  UserData_Init()
功能说明：  用户数据初始化
参   数：   FristLevel 不恢复433信道
            SecondLevel 恢复433信道
返 回 值：  无
*******************************************************************************/
void UserEEPROMData_Init(DataInitLevel_t level)
{
    BF_INT08U tmp[] = {1, 0, 0xF8, 1, 0x53};
	
    SysEEPROMData_Init();
    UnitPublicEEPROMData_Init();
    UnitPrivateEEPROMData_Init();

    if (SecondLevel == level)
    {
        Set_433Channel_Attr((UnitPara_t *)tmp);//恢复出厂设置时，恢复到83信道
    }

    Upload_Fault(USER_DATA_RESET);  			//用户数据重置
}

/*******************************************************************************
函 数 名：  SysEEPROMData_Init()
功能说明：  系统EEPROM数据初始化
参   数：   无
返 回 值：  无
*******************************************************************************/
void SysEEPROMData_Init(void)
{
    gSysData.userID     = DEFAULT_USERID;   //用户号
    gSysData.deviceID   = HardID_Check(0);      //
    gSysData.sendCnt    = 2; //默认两次发送信号，周旺修改，2015.7.2
    gSysData.serialNum  = 0;
    memset((BF_INT08U *)gSysData.BandAddr, 0, sizeof(gSysData.BandAddr)); //清绑定地址转发功能，周旺增加
    gSysData.BandAreaEn = 0; //清区域广播转发功能，周旺增加
    WriteDataToEEPROM(SYSDATA_START_ADD, sizeof(gSysData), (BF_INT08U *)&gSysData);
}

/*******************************************************************************
函 数 名：  SysEEPROMData_Init()
功能说明：  系统EEPROM数据初始化
参   数：   size:   设置数据大小
            data:   设置数据内容
返 回 值：  无
*******************************************************************************/
BF_INT08U SysEEPROMData_Set(BF_INT08U size , BF_INT08U *data)
{
    return WriteDataToEEPROM(SYSDATA_START_ADD + ((BF_INT32U)data - (BF_INT32U)&gSysData), size, data);
}

/*******************************************************************************
函 数 名：  UnitPrivateEEPROMData_Init
功能说明：  单元私有EEPROM数据初始化
参   数：   无
返 回 值：  无
*******************************************************************************/
void UnitPrivateEEPROMData_Init(void)
{
    BF_INT08U unitCnt;

    for (unitCnt = 0; unitCnt < UnitCnt_Get(); unitCnt++)
    {
        UnitPrivateEEPROMUnitData_Init(unitCnt);
    }
}

/*******************************************************************************
函 数 名：  UnitPrivateEEPROMUnitData_Init
功能说明：  单元私有EEPROM单元数据初始化
参   数：   unit： 目标格式化单元号
返 回 值：  无
*******************************************************************************/
void UnitPrivateEEPROMUnitData_Init(BF_INT08U unit)
{
    if (UnitTab[unit].DefEE != NULL)
    {
        UnitTab[unit].DefEE(unit);
    }
}

/*******************************************************************************
函 数 名：  UnitPublicEEPROMData_Init
功能说明：  单元共用EEPROM数据初始化
参   数：   无
返 回 值：  无
*******************************************************************************/
void UnitPublicEEPROMData_Init(void)
{
    BF_INT08U unitCnt;

    for (unitCnt = 0; unitCnt < UnitCnt_Get(); unitCnt++)
    {
        UnitPublicEEPROMUnitData_Init(unitCnt);
    }
}

/*******************************************************************************
函 数 名：  UnitPublicEEPROMUnitData_Init
功能说明：  单元共用EEPROM单元数据初始化
参   数：   unit： 目标格式化单元号
返 回 值：  无
*******************************************************************************/
void UnitPublicEEPROMUnitData_Init(BF_INT08U unit)
{
    *(BF_INT32U*)gUnitData[unit].area = DEFAULT_AREA;	//默认区域号
    gUnitData[unit].type = 0;							//默认设备应用类型
    gUnitData[unit].able = 1;							//默认使能标识
    gUnitData[unit].common = 0;							//默认单元应用大类

    WriteDataToEEPROM(UNITDATA_START_ADD + ONCEUNITDATA_SPACE*unit,\
                      sizeof(UnitData_t), (BF_INT08U*)&gUnitData[unit]);
}

/*******************************************************************************
函 数 名：  UnitPublicEEPROMData_Get
功能说明：  获取单元共用EEPROM数据
参   数：   unitCnt:    获取单元总数
            unitData:   单元数据内容
返 回 值：  无
*******************************************************************************/
void UnitPublicEEPROMData_Get(BF_INT08U unitCnt, UnitData_t *unitData)
{
    while (unitCnt--)
    {
        ReadDataFromEEPROM(UNITDATA_START_ADD + (ONCEUNITDATA_SPACE * (unitCnt)),
                           sizeof(UnitData_t), (BF_INT08U *)&unitData[unitCnt]);
    }
}

/*******************************************************************************
函 数 名：  UnitPublicEEPROMData_Set
功能说明：  设置单元共用EEPROM数据初始化
参   数：   size:   设置数据大小
            data:   设置数据内容
返 回 值：  无
*******************************************************************************/
BF_INT08U UnitPublicEEPROMData_Set(BF_INT08U size , BF_INT08U *data)
{
    BF_INT32U seat = (BF_INT32U)data - (BF_INT32U)&gUnitData;

    return WriteDataToEEPROM(UNITDATA_START_ADD + (ONCEUNITDATA_SPACE * (seat / sizeof(UnitData_t)))
                             + seat % sizeof(UnitData_t), size, data);
}

/**************************Copyright BestFu 2014-05-14*************************/
