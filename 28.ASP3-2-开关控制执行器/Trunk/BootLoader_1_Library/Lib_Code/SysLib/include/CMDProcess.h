/***************************Copyright BestFu 2014-05-14*************************
文	件：	cmdProcess.h
说	明：	FIFO指令处理函数头文件
编	译：	Keil uVision4 V4.54.0.0
版	本：	v1.0
编	写：	Unarty
日	期：	2013-11-01
修  改:     暂无
*******************************************************************************/
#ifndef __CMDPROCESS_H
#define __CMDPROCESS_H

/********************************宏定义****************************************/
//定义Process初始化宏
#define   PROCESS_INIT(pStruct,_flag,_con,_msgfrom,_msggetfun,_prcfun)	do{(pStruct)->flag = (_flag);\
																		(pStruct)->con = (_con);\
																		(pStruct)->msgsrc = (_msgfrom);\
																		(pStruct)->msggetfun = (_msggetfun);\
																		(pStruct)->procfun = (_prcfun);\
																		}while(0)

/*********************************数据类型定义*********************************/
typedef BF_BOOLEAN (*ProcessFunction)(BF_INT08U *buf, BF_INT08U len, BF_INT08U level);
typedef BF_INT32U  (*DecryptGetFunction)(BF_INT08U *cmdBuf, FIFO_t *fifo);

typedef struct
{
	BF_INT08U len; 			//sendBuf处理数据长度
	BF_INT08U flag;			//处理标志
	BF_INT08U con;			//处理条件
	BF_INT08U mean;			//条件描述
	BF_INT08U level;		//消息等级
	BF_INT08U msgsrc;		//消息来源
	BF_INT08U data[255];	//要处理的实际数据内容
	ProcessFunction procfun;
	DecryptGetFunction msggetfun;
}Process_t;

typedef enum
{
	NORMAL,		//正常模式
	MANY_P,		//多次处理
}CMDcon_t;

/*********************************数据声明*************************************/


/*********************************函数声明*************************************/
void FIFOInit(void);
void Process_FIFOData(FIFO_t *fifo, Process_t *prc);
void MsgReviceProc(void);
BF_INT08U Msg_Change(BF_INT08U *data, BF_INT08U len, BF_INT08U level);
MsgResult_t Msg_Execute(CMDPara_t *para,const BF_INT08U msgfrom);
void Msg_Feedback(Communication_t *pMsg,const BF_INT08U msgfrom);

#endif
/**************************Copyright BestFu 2014-05-14*************************/
