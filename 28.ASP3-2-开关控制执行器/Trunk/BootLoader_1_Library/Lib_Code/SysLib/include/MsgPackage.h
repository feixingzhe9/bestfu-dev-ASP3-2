/***************************Copyright BestFu 2014-05-14*************************
文	件：	MsgPackage.h
说	明：	设备自身消息反馈相关函数头文件
编	译：	Keil uVision4 V4.54.0.0
版	本：	v1.0
编	写：	Unarty
日	期：	2014.06.24 
修　改：	暂无
*******************************************************************************/
#ifndef __MSGPACKAGE_H
#define __MSGPACKAGE_H


/*********************************自定义宏*************************************/



/**********************************函数声明***********************************/  
void Msg_Upload(BF_INT08U unit, BF_INT08U cmd, BF_INT08U len, BF_INT08U *data);
void Msg_Send(ObjectType_t objecttype, BF_INT32U objectID, BF_INT16U actNum, BF_INT08U unit, BF_INT08U cmd, BF_INT08U len, BF_INT08U *data);
void Msg_UploadUnit(BF_INT08U unit, BF_INT08U cmd, BF_INT08U len, BF_INT08U *data);


#endif //MsgPackage.h end 
/**************************Copyright BestFu 2014-05-14*************************/
