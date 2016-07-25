/***************************Copyright BestFu 2014-05-14*************************
文	件：	Upload.c
说	明：	设备主动上报相关函数
编	译：	Keil uVision4 V4.54.0.0
版	本：	v1.0
编	写：	Unarty
日	期：	2014.07.03
修　改：	暂无
*******************************************************************************/
#include "lib_type.h"
#include "lib_config.h"
#include "Thread.h"
#include "PivotalDataType.h"
#include "MsgPackage.h"
#include "Upload.h"

//Private variable define
static BF_INT08U Fault;

//Private Function declaration
static void FaultMsg(void);

/*******************************************************************************
函 数 名：	Upload_Fault
功能说明： 	主动异常上报
参	  数： 	upload: 异常情况具体见异常枚举歌词
返 回 值：	无
*******************************************************************************/
void Upload_Fault(Upload_t upload)
{
    BF_INT32U time;

    Fault = (BF_INT08U)upload;
    if (upload == USER_DATA_RESET)
    {
        time = 1000;
    }
    else
    {
        time = 5;
    }
    Thread_Login(ONCEDELAY, 0, time, &FaultMsg);
}
/*******************************************************************************
函 数 名：	FaultMsg
功能说明： 	异常消息封装
参	  数： 	upload: 异常情况具体见异常枚举歌词
返 回 值：	无
*******************************************************************************/
static void FaultMsg(void)
{
    Msg_Upload(0, 0xFF, 1, (BF_INT08U*)&Fault);
}
/**************************Copyright BestFu 2014-05-14*************************/
