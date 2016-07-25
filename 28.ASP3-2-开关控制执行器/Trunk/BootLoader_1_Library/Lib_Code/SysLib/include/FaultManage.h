/***************************Copyright BestFu 2014-05-14*************************
文	件：	FaultManage.h
说	明：	异常处理头文件
编	译：	Keil uVision4 V4.54.0.0
版	本：	v1.0
编	写：	Unarty
日	期：	2014.06.30  
修  改:     暂无
*******************************************************************************/
#ifndef __FAULTMANAGE_H
#define __FAULTMANAGE_H

/*********************************数据类型定义*********************************/
typedef enum
{
	FAULT_0,	//异常等级最高, 设置无法正常使用
	FAULT_1,	//通过重置可以恢复
	FAULT_2,	//通过fun函数指向的内容可以复位
}FaultGrade_t;	//异常等级

typedef enum
{ 
	EEPROM_W_ERR,
	EEPROM_R_ERR,
	SEND_433_FIFO_OVER,			//发送433队列溢出
	RECV_433_FIFO_OVER,			//接收433队列溢出
	SI4432_TX_ERR,				//无线数据发送错误
	SI4432_RX_ERR,				//无线数据接收错误
	PVD_ERR,	       			//PVD异常（暂未加入）
	DEV_RESTART,       			//程序复位
	SEND_USART_FIFO_OVER,		//发送433队列溢出
	RECV_USART_FIFO_OVER,		//接收433队列溢出
	SEND_USB_FIFO_OVER,			//发送433队列溢出
	RECV_USB_FIFO_OVER,			//接收433队列溢出
	SEND_INTERNET_FIFO_OVER,	//发送433队列溢出
	RECV_INTERNET_FIFO_OVER,	//接收433队列溢出	
}Fault_t;

/**********************************函数声明***********************************/
void Fault_Upload(FaultGrade_t grade, Fault_t event, void *fun);
void FaultData_Get(BF_INT08U *len, BF_INT08U *data);
void FaultData_Save(BF_INT08U event);
void FaultData_Init(void);

#endif		//FaultManage.h end
/**************************Copyright BestFu 2014-05-14*************************/
