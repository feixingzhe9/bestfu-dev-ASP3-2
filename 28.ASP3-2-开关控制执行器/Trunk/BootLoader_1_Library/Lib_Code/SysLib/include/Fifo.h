/***************************Copyright BestFu 2016-04-13*************************
文	件：	Fifo.h
说	明：	通用队列处理头文件
编	译：	uVision V5.12.0
版	本：	v1.0
编	写：	Unarty
日	期：	2013-09-03
修  改:     jay: 2016-04-13日修改
*******************************************************************************/

#ifndef __FIFO_H
#define __FIFO_H

/********************************数据结构定义**********************************/
typedef struct{
    BF_INT08U   *data;
    BF_INT32U   size;
    BF_INT32U   front;
    BF_INT32U   rear;
}FIFO_t;

/**********************************函数声明***********************************/                         
BF_BOOLEAN fifo_Init(FIFO_t *head, BF_INT08U *buf, BF_INT32U len);//队列初始化
void fifo_Rst(FIFO_t *head); //清空队列
BF_INT32U fifo_empty(FIFO_t *head); //队空判断
BF_INT32U fifo_validSize(FIFO_t *head);//数据可用空间大小
BF_BOOLEAN fifo_puts(FIFO_t *head, BF_INT08U *data, BF_INT32U len);
BF_BOOLEAN fifo_gets(FIFO_t *head, BF_INT08U *data, BF_INT32U len);
BF_BOOLEAN fifo_putc(FIFO_t *head, BF_INT08U data);
BF_BOOLEAN fifo_getc(FIFO_t *head, BF_INT08U *data);
BF_INT32U fifo_find(FIFO_t *head, const BF_INT08U *data, BF_INT32U len);
BF_BOOLEAN fifo_cmp(const FIFO_t *fifo, BF_INT32U seat, const BF_INT08U *cmp, BF_INT32U cmpsize);
							
#endif //queue.h end
/**************************Copyright BestFu 2016-04-13*************************/
