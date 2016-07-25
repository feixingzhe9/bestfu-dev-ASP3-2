/***************************Copyright BestFu 2016-04-14*************************
文	件：	FifoDataProc.c
说	明：	定义FIFO接口文件
编	译：	uVision V5.12.0
版	本：	v1.0
编	写：	jay
日	期：	2016-04-14
修  改:     NONE
*******************************************************************************/
#include "lib_type.h"
#include "lib_config.h"
#include "crc.h"
#include "fifo.h"
#include "BestFuLib.h"
#include "DataEncrypt.h"
#include "PivotalDataType.h"
#include "FifoDataProc.h"


/*******************************************************************************
函 数 名：  head_put
功能说明：  队列中放置一个数据标识头
参	  数：  fifo:	目标队列
返 回 值：  放置结果，TRUR(放置成功)/FASLE
*******************************************************************************/
BF_BOOLEAN head_put(FIFO_t *fifo)
{
    return fifo_puts(fifo, (BF_INT08U*)HeadCMD, HEAD_SIZE);
}

/*******************************************************************************
函 数 名：  tail_put
功能说明：  队列中放置一个数据标识尾
参	  数：  fifo:	目标队列
返 回 值：  放置结果，TRUR(放置成功)/FASLE
*******************************************************************************/
BF_BOOLEAN tail_put(FIFO_t *fifo)
{
    return fifo_puts(fifo, (BF_INT08U*)TailCMD, TAIL_SIZE);
}

/*******************************************************************************
函数  名:  BF_INT32U decryptfifo_get(BF_INT08U *cmdBuf, FIFO_t *fifo)
功能说明:  从加密队列中获取一帧数据
参    数:  fifo：缓冲队列的首地址
		   cmdBuf：存储返回数据的首地址
返回  值:  无
*******************************************************************************/
BF_INT32U decryptfifo_get(BF_INT08U *cmdBuf, FIFO_t *fifo)
{
    BF_INT32U pos = 0u;

    ERRR(fifo == NULL, goto ERR1);
    ERRR(fifo_validSize(fifo) <= HEAD_SIZE, goto ERR1);				//判断可用有效长度是否大于帧头

    pos = fifo_find(fifo, HeadCMD, HEAD_SIZE);
    fifo->front = pos;

    ERRR(fifo_empty(fifo) == TRUE, goto ERR1);        				//FIFO为空
    pos = fifo->data[(fifo->front + HEAD_SIZE)%fifo->size];			//获取有效指令长度
    ERRR(COMMUNICATION_VERSION >= pos , goto ERR2);	 				//旧通信协议帧直接移除 yanhuan adding 2016/01/13
    pos += HEAD_SIZE + TAIL_SIZE;									//获取参数区总长度
    ERRR(pos > MAX_DATA_FRAME_LEN, goto ERR2);						//数据指令长度错误
    ERRR(pos > fifo_validSize(fifo), goto ERR1); 					//数据空间不够

    ERRR(fifo_cmp(fifo, (fifo->front + pos - TAIL_SIZE)%fifo->size\
                  , TailCMD, TAIL_SIZE) == FALSE, goto ERR2);		//核对数据尾
    ERRR(fifo_gets(fifo, cmdBuf, pos) == FALSE, goto ERR1);			//数据内容出队

    return pos;

ERR2:
    fifo->front = (fifo->front + HEAD_SIZE)%fifo->size; 		//数据错误将头移出
ERR1:
    return 0;
}

/*******************************************************************************
函数  名:  BF_INT32U decryptfifo_getEx(BF_INT08U *cmdBuf, FIFO_t *fifo)
功能说明:  从加密队列中获取一帧数据,并解密数据
参    数:  fifo：缓冲队列的首地址
		   cmdBuf：存储返回数据的首地址
返回  值:  无
*******************************************************************************/
BF_INT32U decryptfifo_getEx(BF_INT08U *cmdBuf, FIFO_t *fifo)
{
    BF_INT32U pos = decryptfifo_get(cmdBuf, fifo);

	if(pos && (!DecryptData((BF_INT08U*)(&cmdBuf[HEAD_SIZE]))))	//若解密成功
	{
		pos = HEAD_SIZE + sizeof(Communication_t) + ((Communication_t*)&cmdBuf[HEAD_SIZE])->para.len + CRC_SIZE;
		memcpy(&cmdBuf[pos], (BF_INT08U*)TailCMD, TAIL_SIZE); 
		pos += TAIL_SIZE;
	}
    return (pos);
}

/*******************************************************************************
函 数 名：  EncryptCmd_put
功能说明：  加密一条指令后放置到FIFO中
参	  数：  fifo:	目标地址
            cmdBuf: 指令存储缓冲区
			cmdLen:	放置内容长度
返 回 值：  指令长度。为零，没有可读指令
*******************************************************************************/
BF_INT32U EncryptCmd_put(FIFO_t *fifo, BF_INT08U *cmdBuf, BF_INT32U cmdLen)
{
    BF_INT16U crc;
	BF_INT08U cmdBuf_temp[255];
	
	CPU_SR_ALLOC();
    ERRR(NULL == fifo, goto ERR1);
    ERRR(cmdLen > MAX_DATA_FRAME_LEN, goto ERR1);

	memcpy(cmdBuf_temp, cmdBuf, cmdLen);
    crc = ChkCrcValue(cmdBuf_temp, cmdLen);				//获取CRC
	ENTER_CRITICAL();
    ERRR(fifo_puts(fifo, (BF_INT08U*)HeadCMD, sizeof(HeadCMD)) == FALSE, goto ERR1);	//放置通信标志头
    cmdBuf_temp[cmdLen++] = (BF_INT08U)(crc >> 8);
    cmdBuf_temp[cmdLen++] = (BF_INT08U)(crc);			//添加数据CRC
	
    ERRR(!EncryptData(cmdBuf_temp, cmdLen), goto ERR1);	//加密失败,返回假
    cmdLen = cmdBuf_temp[0];
    ERRR(cmdLen > MAX_DATA_FRAME_LEN, goto ERR1);
    ERRR(fifo_puts(fifo, cmdBuf_temp, cmdLen) == FALSE, goto ERR1);	//放置参数区
    ERRR(fifo_puts(fifo, (BF_INT08U*)TailCMD, sizeof(TailCMD)) == FALSE, goto ERR1);	//放置通信标尾
	EXIT_CRITICAL();
	
    return TRUE;
ERR1:
    return FALSE;
}

/**************************Copyright BestFu 2016-05-14*************************/
