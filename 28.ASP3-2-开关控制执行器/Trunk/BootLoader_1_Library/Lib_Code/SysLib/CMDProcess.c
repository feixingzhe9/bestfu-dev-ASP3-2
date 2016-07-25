/***************************Copyright BestFu 2014-05-14*************************
文	件：    cmdProcess.c
说	明：    FIFO指令处理函数
编	译：    Keil uVision4 V4.54.0.0
版	本：    v1.0
编	写：    Unarty
日	期：    2013-11-01
修  改:     2014.12.28 Unarty 修改单元属性执行边界条件判断值，防止多单元执行时属性越界
			2014.01.08 Unarty 增加单元数值核对 防止访问越界
*******************************************************************************/
#include "lib_type.h"
#include "lib_config.h"
#include "PivotalDataType.h"
#include "fifo.h"
#include "Thread.h"
#include "UserData.h"
#include "BestFuLib.h"
#include "Object.h"
#include "FifoDataProc.h"
#include "FaultManage.h"
#include "PhyMsgProc.h"
#include "UnitCfg.h"
#include "CMDProcess.h"

//Globe variable define


//Private variable define
#if (RF_433M_INTERFACE_EN > 0u)
Process_t rcv433Prc;
Process_t snd433Prc;

#endif
#if (USART_INTERFACE_EN > 0u )
Process_t rcvUsartPrc;
Process_t sndUsartPrc;

#endif
#if (USB_INTERFACE_EN > 0u)
Process_t rcvUSBPrc;
Process_t sndUSBPrc;

#endif
#if (INTERNET_INTERFACE_EN > 0u)
Process_t rcvIntnetPrc;
Process_t sndIntnetPrc;

#endif

//Private function declareation
static BF_INT08U   Msg_ReturnType(BF_INT08U msgType);
static void DataSend_Thread(void);
//static BF_INT08U   Msg_Level(BF_INT32U object);		////用于通信匹配（Unarty 2014.08.07)
static BF_BOOLEAN Msg_Process(BF_INT08U *sdata, BF_INT08U len, BF_INT08U level);

/*****************************************************************************
  函数名:  void sendFIFOInit(void)
功能说明: 	发送FIFO队列初始化
    参数:  	void
  返回值:  	void
*****************************************************************************/
void FIFOInit(void)
{
    PHY_RcvFIFO_Init();//   MSG_FROM_433

#if (RF_433M_INTERFACE_EN > 0u)
    PROCESS_INIT(&rcv433Prc,0,NORMAL,RF_433M_INTERFACE,decryptfifo_getEx,Msg_Process);			//接收数据正常处理
    PROCESS_INIT(&snd433Prc,0,MANY_P,RF_433M_INTERFACE,decryptfifo_get,PHY_433Send);			//发送处理

#endif
#if (USART_INTERFACE_EN > 0u )
    PROCESS_INIT(&rcvUsartPrc,0,NORMAL,USART_INTERFACE,decryptfifo_getEx,Msg_Process);			//接收数据正常处理
    PROCESS_INIT(&sndUsartPrc,0,NORMAL,USART_INTERFACE,decryptfifo_get,PHY_USARTSend);			//发送处理

#endif
#if (USB_INTERFACE_EN > 0u)
    PROCESS_INIT(&rcvUSBPrc,0,NORMAL,USB_INTERFACE,decryptfifo_getEx,Msg_Process);				//接收数据正常处理
    PROCESS_INIT(&sndUSBPrc,0,NORMAL,USB_INTERFACE,decryptfifo_get,PHY_USBTSend);				//发送处理

#endif
#if (INTERNET_INTERFACE_EN > 0u)
    PROCESS_INIT(&rcvIntnetPrc,0,NORMAL,INTERNET_INTERFACE,decryptfifo_getEx,Msg_Process);		//接收数据正常处理
    PROCESS_INIT(&sndIntnetPrc,0,NORMAL,INTERNET_INTERFACE,decryptfifo_get,PHY_INTERNETTSend);	//发送处理

#endif
    Thread_Login(FOREVER, 0, 5, &DataSend_Thread);
}

/*****************************************************************************
  函数名:  	DataSend_Thread
功能说明: 	数据发送线程
	参数:  	void
  返回值:  	void
*****************************************************************************/
void MsgReviceProc(void)
{
#if (RF_433M_INTERFACE_EN > 0u)
    Process_FIFOData(&phy_rcv433fifo, &rcv433Prc);		//处理接收FIFO

#endif
#if (USART_INTERFACE_EN > 0u )
    Process_FIFOData(&phy_rcvUsartfifo, &rcvUsartPrc);	//处理接收FIFO

#endif
#if (USB_INTERFACE_EN > 0u)
    Process_FIFOData(&phy_rcvUSBfifo, &rcvUSBPrc);		//处理接收FIFO

#endif
#if (INTERNET_INTERFACE_EN > 0u)
    Process_FIFOData(&rcvIntnetPrc, &phy_rcvIntnetfifo);//处理接收FIFO

#endif
}

/*****************************************************************************
  函数名:  	DataSend_Thread
功能说明: 	数据发送线程
	参数:  	void
  返回值:  	void
*****************************************************************************/
void DataSend_Thread(void)
{
#if (RF_433M_INTERFACE_EN > 0u)
    Process_FIFOData(&phy_snd433fifo, &snd433Prc);		//处理发送FIFO

#endif
#if (USART_INTERFACE_EN > 0u )
    Process_FIFOData(&phy_sndUsartfifo, &sndUsartPrc);	//处理发送FIFO

#endif
#if (USB_INTERFACE_EN > 0u)
    Process_FIFOData(&phy_sndUSBfifo, &sndUSBPrc);		//处理发送FIFO

#endif
#if (INTERNET_INTERFACE_EN > 0u)
    Process_FIFOData(&sndIntnetPrc, &phy_sndIntnetfifo);//处理发送FIFO

#endif
}

/*******************************************************************
函 数 名：	Send_FIFOData
功能说明： 	将433队列中的数据通过433发送出去
参	  数： 	fifo:	操作目标FIFO
			prc:	过程处理函数
返 回 值：	TRUE/FALSE
*******************************************************************/
void Process_FIFOData(FIFO_t *fifo, Process_t *prc)
{
    switch (prc->flag)
    {
    case 0:
    {
        if (0 == (prc->len = prc->msggetfun(prc->data, fifo)))   			//从FIFO中获取一条指令失败
        {
            break;
        }
        prc->level = 0;//Msg_Level(((Communication_t*)&prc->data[HEAD_SIZE])->aim.object.id);
        prc->mean = (prc->con == MANY_P ? gSysData.sendCnt : 1);			//获取数据处理次数
    }
    case 1:
    {
        if (FALSE == (prc->procfun(prc->data, prc->len, prc->level)) || --prc->mean)   //数据处理还未完成
        {
            prc->flag = 1;
            break;
        }
    }
    default:
    {
        prc->flag = 0;
    }
    break;
    }
}

/*******************************************************************
函 数 名：	Msg_Process
功能说明： 	消息处理函数
参	  数： 	sdata	:	整个通讯帧明文数据首地址
			len		:	数据帧的长度
			level	:发送消息的延时级别
返 回 值：	TRUE/FALSE
*******************************************************************/
static BF_BOOLEAN Msg_Process(BF_INT08U *sdata, BF_INT08U len, BF_INT08U level)
{
    MsgResult_t result;
    const BF_INT08U msgfrom = *(sdata - 1);
    Communication_t *pMsg;
    pMsg = (Communication_t*)&sdata[HEAD_SIZE];
    //核对通信版本	pMsg->version;
    if ((result = Object_Check(sdata, len, &pMsg->aim, &pMsg->para,\
                               &sdata[HEAD_SIZE+sizeof(Communication_t)])) == COMPLETE)   //目标核对成功
    {
        /*指令执行 pMsg->para ; data*/
        result =  Msg_Execute(&pMsg->para, msgfrom);
        /*指令反馈*/
        if (result != NO_ACK)
        {
            if (WRITEWITHACK == pMsg->para.msgType									//写应答
                    || READWITHACK == pMsg->para.msgType							//读应答
                    || ((WRITESUC == pMsg->para.msgType
                         || READSUC == pMsg->para.msgType) && result == COMPLETE) 	//写/读成功应答并且操作成功
                    || ((WRITEFAIL == pMsg->para.msgType
                         || READFAIL == pMsg->para.msgType) && result != COMPLETE)) //写/读失败应答并且操作失败
            {
                //消息需要应答
                pMsg->para.msgType = Msg_ReturnType(pMsg->para.msgType);			//消息执行结果
                Msg_Feedback(pMsg, msgfrom);
            }
        }
    }
    return TRUE;
}

/*******************************************************************************
函 数 名：	Msg_Feedback
功能说明： 	消息反馈
参	  数： 	pMsg:	消息内容
返 回 值：	TRUE(重复)/FALSE(不重复)
*******************************************************************************/
void Msg_Feedback(Communication_t *pMsg,const BF_INT08U msgfrom)
{
    const MsgSrcFIFOPtr_t* pMsgSrcFIFO = GetCurrentSrcFifoPtr(msgfrom);
	
	pMsg->version = COMMUNICATION_VERSION;
    pMsg->aim.userID = gSysData.userID;
    pMsg->aim.sourceID = gSysData.deviceID;

	//若为广播消息或者发送目标是自己的情况,均要压入自己的接收队列
    if ((pMsg->aim.object.id == gSysData.deviceID) ||  (pMsg->aim.objectType > SINGLE_ACT))
    {
		if(!EncryptCmd_put(pMsgSrcFIFO->rcvfifoPtr, (BF_INT08U*)pMsg, sizeof(Communication_t) + pMsg->para.len))
		{
			Fault_Upload(FAULT_2 , pMsgSrcFIFO->rcvfault , NULL);	//暂时不做处理
		}
    }
	//目标ID不是自己
    if(pMsg->aim.object.id != gSysData.deviceID)  
    {
        if(!EncryptCmd_put(pMsgSrcFIFO->sndfifoPtr, (BF_INT08U*)pMsg, sizeof(Communication_t) + pMsg->para.len))
		{
			Fault_Upload(FAULT_2 , pMsgSrcFIFO->sndfault , NULL);	//暂时不做处理
		}
    }
}

/*******************************************************************
函 数 名：	Msg_Execute
功能说明： 	消息执行
参	  数： 	data:	要执行的数据内容
			len;	数据长度
返 回 值：	TRUE(重复)/FALSE(不重复)
*******************************************************************/
MsgResult_t Msg_Execute(CMDPara_t *para,const BF_INT08U msgfrom)
{
    MsgResult_t result = NO_CMD;
    const AttrExe_st *pAttrExe = UnitTab[para->unit].attrExe;
    BF_INT08U ExeAttrNum = 0u;
    BF_INT08U ackLen = 0;

    if (para->unit < UnitCnt_Get())  		//指令单元没有越界	2014.01.08 Unarty add 防止访问越界
    {
        do   //遍历属性列表
        {
            if (pAttrExe->cmd == para->cmd)  	//属性号匹配OK
            {
                /*写操作*/
                if (WRITEWITHACK == para->msgType
                        || WRITENACK == para->msgType
                        || WRITESUC  == para->msgType
                        || WRITEFAIL == para->msgType)
                {
                    if (pAttrExe->pSet_Attr != NULL)
                    {
                        if (!gUnitData[para->unit].able)  	//单元被禁用
                        {
                            result = UNIT_DISABLE;
                        }
                        else
                        {
                            memmove(&para->cmd,&para->unit,3 + para->len);
							para->unit = msgfrom;
							result =  pAttrExe->pSet_Attr((UnitPara_t*)&para->unit);
							memmove(&para->unit,&para->cmd, 3);
                            if (result == OWN_RETURN_PARA)   //如果写入结里带返回参数
                            {
                                if (pAttrExe->cmd > 0xCF)
                                {
                                    result = COMPLETE;
                                }
                                ackLen = para->len;                          
                            }
                        }
                    }
                    else
                    {
                        result = NO_WRITE;	//没有写权限
                    }
                }
                /*读操作*/
                else if (READWITHACK == para->msgType
                         || READSUC == para->msgType
                         || READFAIL == para->msgType)
                {
                    if (pAttrExe->pGet_Attr != NULL)
                    {
                        memmove(&para->cmd,&para->unit,3 + para->len);
						para->unit = msgfrom;
						result = pAttrExe->pGet_Attr((UnitPara_t*)&para->unit, &ackLen, ((&para->len) + 2));
						memmove(&para->unit, &para->cmd, 3);
                    }
                    else
                    {
                        result = NO_READ;	//没有读权限
                    }
                }
                else
                {
                    result = MSG_TYPE_ERR;	//消息类型错误
                }
                break;
            }
        }
        while (((++ExeAttrNum) < UnitTab[para->unit].ExeAttrNum())    //2016.01.29 jay modify
                &&((++pAttrExe)->cmd < 0x0100));
    }

    para->len = ackLen + 1;
    *((&para->len)+1) = result;		//V1.8 ADD

    return result;
}

/*******************************************************************
函 数 名：	Msg_ReturnType
功能说明： 	消息返回类型
参	  数： 	执行消息类型
返 回 值：	返回消息应答类型
*******************************************************************/
BF_INT08U Msg_ReturnType(BF_INT08U msgType)
{
    switch (msgType)
    {
    case WRITEWITHACK:
    case WRITESUC:
    case WRITEFAIL:
        return ACKWRITE;
    case READWITHACK:
    case READSUC:
    case READFAIL:
        return READACK;
    default :
        break;
    }
    return 0;
}

///*******************************************************************
//函 数 名：	Msg_Level
//功能说明： 	消息执行等级。主要用数据发送
//参	  数： 	消息目标
//返 回 值：	消息等级
//*******************************************************************/
//BF_INT08U Msg_Level(BF_INT32U object)
//{
//    if (MSG_UPLOAD_ID == object)  	//数据上报
//    {
//        return 2;
//    }
//    else if (DelayID == object)  	//目标操作可以延时应答
//    {
//        return 3;
//    }
//    return 0;
//}
/**************************Copyright BestFu 2014-05-14*************************/
