/***************************Copyright BestFu 2016-04-14*************************
文	件：	PhyMsgProc.c
说	明：	物理层收发消息预处理文件
编	译：	uVision V5.12.0
版	本：	v1.0
编	写：	jay
日	期：	2016-04-14
修  改:     NONE
*******************************************************************************/
#include "lib_type.h"
#include "lib_config.h"
#include "SI4432.h"
#include "fifo.h"
#include "crc.h"
#include "iwdg.h"
#include "DataEncrypt.h"
#include "PivotalDataType.h"
#include "FaultManage.h"
#include "BestFuLib.h"
#include "FifoDataProc.h"
#include "PhyMsgProc.h"
#include "usart.h"

//using save message source pointer
static MsgSrcFIFOPtr_t MsgSrcFIFOPtr[4];

//variable define
#if (RF_433M_INTERFACE_EN > 0u)
FIFO_t phy_rcv433fifo;
FIFO_t phy_snd433fifo;
static BF_INT08U phy_rcv433Buf[RF_433M_INTERFACE_BUF_SIZE];
static BF_INT08U phy_snd433Buf[RF_433M_INTERFACE_BUF_SIZE];

#endif
#if (USART_INTERFACE_EN > 0u )
FIFO_t phy_rcvUsartfifo;
FIFO_t phy_sndUsartfifo;
static BF_INT08U phy_rcvUsartBuf[USART_INTERFACE_BUF_SIZE];
static BF_INT08U phy_sndUsartBuf[USART_INTERFACE_BUF_SIZE];

#endif
#if (USB_INTERFACE_EN > 0u)
FIFO_t phy_rcvUSBfifo;
FIFO_t phy_sndUSBfifo;
static BF_INT08U phy_rcvUSBBuf[USB_INTERFACE_BUF_SIZE];
static BF_INT08U phy_sndUSBBuf[USB_INTERFACE_BUF_SIZE];

#endif
#if (INTERNET_INTERFACE_EN > 0u)
FIFO_t phy_rcvIntnetfifo;
FIFO_t phy_sndIntnetfifo;
static BF_INT08U phy_rcvInternetBuf[INTERNET_INTERFACE_BUF_SIZE];
static BF_INT08U phy_sndInternetBuf[INTERNET_INTERFACE_BUF_SIZE];

#endif

/*******************************************************************************
函 数 名：  PHY_RcvFIFO_Init
功能说明：  PHY层接收队列初始化
参	  数：  NONE
返 回 值：  初始化结果 TRUE/FALSE
*******************************************************************************/
void PHY_RcvFIFO_Init(void)
{
#if (RF_433M_INTERFACE_EN > 0u)
    fifo_Init(&phy_rcv433fifo,phy_rcv433Buf,RF_433M_INTERFACE_BUF_SIZE);
    fifo_Init(&phy_snd433fifo,phy_snd433Buf,RF_433M_INTERFACE_BUF_SIZE);

#endif
#if (USART_INTERFACE_EN > 0u )
    fifo_Init(&phy_rcvUsartfifo, phy_rcvUsartBuf,USART_INTERFACE_BUF_SIZE);
    fifo_Init(&phy_sndUsartfifo, phy_sndUsartBuf,USART_INTERFACE_BUF_SIZE);

#endif
#if (INTERNET_INTERFACE_EN > 0u)
    fifo_Init(&phy_rcvUSBfifo, phy_rcvUSBBuf,USB_INTERFACE_BUF_SIZE);
    fifo_Init(&phy_sndUSBfifo, phy_sndUSBBuf,USB_INTERFACE_BUF_SIZE);

#endif
#if (INTERNET_INTERFACE_EN > 0u)
    fifo_Init(&phy_rcvIntnetfifo, phy_rcvInternetBuf,INTERNET_INTERFACE_BUF_SIZE);
    fifo_Init(&phy_sndIntnetfifo, phy_sndInternetBuf,INTERNET_INTERFACE_BUF_SIZE);

#endif
}

/*******************************************************************************
函 数 名：	GetCurrentSrcFifoPtr
功能说明： 	获取当前消息源接收发送fifo
参	  数： 	msgfrom	:消息来源		
返 回 值：	返回保存当前消息来源fifo的数据结构地址
说    明：	此函数中断不可使用
*******************************************************************************/
MsgSrcFIFOPtr_t*  GetCurrentSrcFifoPtr(const BF_INT08U msgfrom)
{
	MsgSrcFIFOPtr_t* pMsgSrcFIFO;
	
	switch(msgfrom)
	{
#if (USART_INTERFACE_EN > 0u )
	case USART_INTERFACE:
	{
		MsgSrcFIFOPtr[0].rcvfault	 = RECV_USART_FIFO_OVER;
		MsgSrcFIFOPtr[0].sndfault	 = SEND_USART_FIFO_OVER;
		MsgSrcFIFOPtr[0].rcvfifoPtr = (FIFO_t*)&phy_rcvUsartfifo;
		MsgSrcFIFOPtr[0].sndfifoPtr = (FIFO_t*)&phy_sndUsartfifo;
		pMsgSrcFIFO = &MsgSrcFIFOPtr[0];
	}
	break;
#endif
#if (USB_INTERFACE_EN > 0u)
	case USB_INTERFACE:
	{
		MsgSrcFIFOPtr[1].rcvfault	 = RECV_USB_FIFO_OVER;
		MsgSrcFIFOPtr[1].sndfault	 = SEND_USB_FIFO_OVER;
		MsgSrcFIFOPtr[1].rcvfifoPtr = (FIFO_t*)&phy_rcvUSBfifo;
		MsgSrcFIFOPtr[1].sndfifoPtr = (FIFO_t*)&phy_sndUSBfifo;
		pMsgSrcFIFO = &MsgSrcFIFOPtr[1];
	} break;
#endif
#if (INTERNET_INTERFACE_EN > 0u)
	case INTERNET_INTERFACE:
	{
		MsgSrcFIFOPtr[2].rcvfault	 = RECV_INTERNET_FIFO_OVER;
		MsgSrcFIFOPtr[2].sndfault	 = SEND_INTERNET_FIFO_OVER;
		MsgSrcFIFOPtr[2].rcvfifoPtr = (FIFO_t*)&phy_rcvIntnetfifo;
		MsgSrcFIFOPtr[2].sndfifoPtr = (FIFO_t*)&phy_sndIntnetfifo;
		pMsgSrcFIFO = &MsgSrcFIFOPtr[2];
	} break;
#endif
#if (RF_433M_INTERFACE_EN > 0u)
	case RF_433M_INTERFACE:
#endif
	default:
	{
		MsgSrcFIFOPtr[3].rcvfault	 = RECV_433_FIFO_OVER;
		MsgSrcFIFOPtr[3].sndfault	 = SEND_433_FIFO_OVER;
		MsgSrcFIFOPtr[3].rcvfifoPtr = (FIFO_t*)&phy_rcv433fifo;
		MsgSrcFIFOPtr[3].sndfifoPtr = (FIFO_t*)&phy_snd433fifo;
		pMsgSrcFIFO = &MsgSrcFIFOPtr[3];
	}
	break;
	}
	return ((MsgSrcFIFOPtr_t*)pMsgSrcFIFO);
}

#if (RF_433M_INTERFACE_EN > 0u)
/*******************************************************************************
函 数 名：  BF_BOOLEAN PHY_433Send(BF_INT08U *buf, BF_INT08U len, BF_INT08U level)
功能说明：  PHY层433消息发送函数
参	  数：  buf:  待发送消息的首指针
            len:  消息的长度
            level:  消息等级
返 回 值：  结果 TRUE/FALSE
*******************************************************************************/
BF_BOOLEAN PHY_433Send(BF_INT08U *buf, BF_INT08U len, BF_INT08U level)
{
    return Si4432_Send(&buf[HEAD_SIZE], len - HEAD_SIZE - TAIL_SIZE, level);
}

#endif

#if (USART_INTERFACE_EN > 0u )
/*******************************************************************************
函 数 名：  BF_BOOLEAN PHY_USARTSend(BF_INT08U *buf, BF_INT08U len, BF_INT08U level)
功能说明：  PHY层串口消息发送函数
参	  数：  buf:  待发送消息的首指针
            len:  消息的长度
            level:  消息等级
返 回 值：  结果 TRUE/FALSE
*******************************************************************************/
BF_BOOLEAN PHY_USARTSend(BF_INT08U *buf, BF_INT08U len, BF_INT08U level)
{

    return USART_Send(buf,len,level);
}

#endif


#if (INTERNET_INTERFACE_EN > 0u)
/*******************************************************************************
函 数 名：  BF_BOOLEAN PHY_USBTSend(BF_INT08U *buf, BF_INT08U len, BF_INT08U level)
功能说明：  PHY层USB消息发送函数
参	  数：  buf:  待发送消息的首指针
            len:  消息的长度
            level:  消息等级
返 回 值：  结果 TRUE/FALSE
*******************************************************************************/
BF_BOOLEAN PHY_USBTSend(BF_INT08U *buf, BF_INT08U len, BF_INT08U level)
{


}
#endif


#if (INTERNET_INTERFACE_EN > 0u)
/*******************************************************************************
函 数 名：  BF_BOOLEAN PHY_INTERNETTSend(BF_INT08U *buf, BF_INT08U len, BF_INT08U level)
功能说明：  PHY层网络消息发送函数
参	  数：  buf:  待发送消息的首指针
            len:  消息的长度
            level:  消息等级
返 回 值：  结果 TRUE/FALSE
*******************************************************************************/
BF_BOOLEAN PHY_INTERNETTSend(BF_INT08U *buf, BF_INT08U len, BF_INT08U level)
{


}

#endif

/**************************Copyright BestFu 2016-05-14*************************/
