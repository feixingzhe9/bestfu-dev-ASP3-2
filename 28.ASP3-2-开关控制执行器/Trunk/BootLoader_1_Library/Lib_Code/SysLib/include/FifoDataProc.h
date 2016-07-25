/***************************Copyright BestFu 2016-04-14*************************
文	件：	FifoDataProc.h
说	明：	fifo数据处理头文件
编	译：	uVision V5.12.0
版	本：	v1.0
编	写：	jay
日	期：	2016-04-14
修  改:     NONE
*******************************************************************************/

#ifndef __FIFO_DATA_PROC_H
#define __FIFO_DATA_PROC_H

/********************************数据结构定义**********************************/


/**********************************函数声明***********************************/   
BF_BOOLEAN head_put(FIFO_t *fifo);
BF_BOOLEAN tail_put(FIFO_t *fifo);
BF_INT32U decryptfifo_get(BF_INT08U *cmdBuf, FIFO_t *fifo);
BF_INT32U EncryptCmd_put(FIFO_t *fifo, BF_INT08U *cmdBuf, BF_INT32U cmdLen);
BF_INT32U decryptfifo_getEx(BF_INT08U *cmdBuf, FIFO_t *fifo);

#endif //FifoDataProc.h end
/**************************Copyright BestFu 2016-04-14*************************/
