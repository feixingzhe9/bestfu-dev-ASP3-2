/***************************Copyright BestFu 2016-04-14*************************
文	件：	PhyMsgProc.h
说	明：	物理层收发消息预处理头文件
编	译：	uVision V5.12.0
版	本：	v1.0
编	写：	jay
日	期：	2016-04-14
修  改:     NONE
*******************************************************************************/

#ifndef __PHY_MSG_PROC_H	
#define __PHY_MSG_PROC_H

/********************************数据结构定义**********************************/
#pragma pack(1)
typedef struct
{
	Fault_t rcvfault;
	Fault_t sndfault;
	FIFO_t* rcvfifoPtr;
	FIFO_t* sndfifoPtr;	
}MsgSrcFIFOPtr_t;
#pragma pack()

/********************************外部变量声明**********************************/
//variable define 
#if (RF_433M_INTERFACE_EN > 0u)
extern FIFO_t phy_rcv433fifo;
extern FIFO_t phy_snd433fifo;

#endif
#if (USART_INTERFACE_EN > 0u )
extern FIFO_t phy_rcvUsartfifo;
extern FIFO_t phy_sndUsartfifo;
	
#endif
#if (USB_INTERFACE_EN > 0u)
extern FIFO_t phy_rcvUSBfifo;
extern FIFO_t phy_sndUSBfifo;
	
#endif
#if (INTERNET_INTERFACE_EN > 0u)
extern FIFO_t phy_rcvIntnetfifo;
extern FIFO_t phy_sndIntnetfifo;

#endif

/**********************************函数声明***********************************/                         
void PHY_RcvFIFO_Init(void);
BF_INT32U PHY_RcvMsgProc(FIFO_t *fifo);
MsgSrcFIFOPtr_t*  GetCurrentSrcFifoPtr(const BF_INT08U msgfrom);

#if (RF_433M_INTERFACE_EN > 0u)
BF_BOOLEAN PHY_433Send(BF_INT08U *buf, BF_INT08U len, BF_INT08U level);

#endif

#if (USART_INTERFACE_EN > 0u )
BF_BOOLEAN PHY_USARTSend(BF_INT08U *buf, BF_INT08U len, BF_INT08U level);

#endif


#if (INTERNET_INTERFACE_EN > 0u)
BF_BOOLEAN PHY_USBTSend(BF_INT08U *buf, BF_INT08U len, BF_INT08U level);

#endif

#if (INTERNET_INTERFACE_EN > 0u)
BF_BOOLEAN PHY_INTERNETTSend(BF_INT08U *buf, BF_INT08U len, BF_INT08U level);
	
#endif

#endif //PreRcvMsg.h end
/**************************Copyright BestFu 2016-04-14*************************/
