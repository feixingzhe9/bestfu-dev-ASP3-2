/***************************Copyright BestFu 2014-05-14*************************
文	件：	MsgPackage.c
说	明：	设备自身消息反馈相关函数
编	译：	Keil uVision4 V4.54.0.0
版	本：	v1.0
编	写：	Unarty
日	期：	2014.06.24
修　改：	暂无
*******************************************************************************/
#include "lib_type.h"
#include "lib_config.h"
#include "BestFuLib.h"
#include "fifo.h"
#include "PivotalDataType.h"
#include "UserData.h"
#include "FifoDataProc.h"
#include "CMDProcess.h"
#include "MsgPackage.h"

//Private function declaration
static void Msg_Package(CMDAim_t *aim, CMDPara_t *para, BF_INT08U *data);

/*******************************************************************************
函 数 名：	Msg_Upload
功能说明： 	消息上报
参	  数： 	unit:	单元号
			cmd：	属性号
			len:	参数长度
			*data:	参数内容
返 回 值：	无
*******************************************************************************/
void Msg_Upload(BF_INT08U unit, BF_INT08U cmd, BF_INT08U len, BF_INT08U *data)
{
    BF_INT08U buf[64];

    buf[0] = COMPLETE;	//增加通信成功标识
    buf[1] = unit;
    buf[2] = cmd;
    buf[3] = len;
    memcpy(&buf[4], data, len);

    Msg_UploadUnit( 0x00, 0x00, len + 4, buf);
}

/*******************************************************************************
函 数 名：	Msg_UploadUnit
功能说明： 	单元消息上报
参	  数： 	unit:	单元号
			cmd：	属性号
			len:	参数长度
			*data:	参数内容
返 回 值：	无
*******************************************************************************/
void Msg_UploadUnit(BF_INT08U unit, BF_INT08U cmd, BF_INT08U len, BF_INT08U *data)
{
    CMDAim_t aim;
    CMDPara_t para;

    aim.objectType = SINGLE_ACT;
    aim.object.id = MSG_UPLOAD_ID;
    aim.actNum = 0;

    para.msgType = EVENT;
    para.unit = unit;
    para.cmd = cmd;
    para.len = len;
    Msg_Package(&aim, &para, data);
}
/*******************************************************************************
函 数 名：	Msg_Send
功能说明： 	消息发送
参	  数： 	objectType:		目标地址类型
			objectID：		目标地址
			actNum:			广播类型号
			unit:			单元号
			cmd：			属性号
			len:			参数长度
			*data:			参数内容
返 回 值：	无
*******************************************************************************/
void Msg_Send(ObjectType_t objecttype, BF_INT32U objectID, BF_INT16U actNum, BF_INT08U unit, BF_INT08U cmd, BF_INT08U len, BF_INT08U *data)
{
    CMDAim_t aim;
    CMDPara_t para;

    aim.objectType = objecttype;
    aim.object.id = objectID;			//设置目标地址
    aim.actNum = actNum;

    /*消息参数区赋值*/
    para.msgType = WRITENACK;			//消息类型，事件
    para.cmd = cmd;
    para.unit = unit;
    para.len = len;
    Msg_Package(&aim, &para, data);
}

/*******************************************************************************
函 数 名：	Msg_Package
功能说明： 	消息打包
参	  数： 	aim:	目标描述
			para:	指令描述
			*data:	参数内容
返 回 值：	无
*******************************************************************************/
void Msg_Package(CMDAim_t *aim, CMDPara_t *para, BF_INT08U *data)
{
    BF_INT08U msg[250];
    Communication_t *pMsg = (Communication_t*)msg;

    memcpy(&pMsg->aim, aim, sizeof(CMDAim_t));
    pMsg->aim.serialNum = ++gSysData.serialNum;		//设置消息流水号

    /*消息参数区赋值*/
    memcpy(&pMsg->para, para, sizeof(CMDPara_t));
    memcpy(&msg[sizeof(Communication_t)], data, para->len);
    Msg_Feedback(pMsg, RF_433M_INTERFACE);
}

/**************************Copyright BestFu 2014-05-14*************************/
