/***************************Copyright BestFu 2014-05-14*************************
文	件：	DeviceUpgradeAttr.h
说	明：	设备升级属性实现头文件
编	译：	uVision V5.12.0
版	本：	v1.0
编	写：	jay
日	期：	2016-04-15
修  改:     暂无
*******************************************************************************/
#ifndef __DEVICE_UPGRADE_H   
#define __DEVICE_UPGRADE_H

/*********************************自定义宏*************************************/


/********************************数据结构定义**********************************/  

/**********************************函数声明***********************************/
MsgResult_t Set_IAPReady_Attr(UnitPara_t *pData);
MsgResult_t Get_IAPReady_Attr(UnitPara_t *pData, BF_INT08U *rlen, BF_INT08U *rpara);
MsgResult_t Set_IAPPackSave_Attr(UnitPara_t *pData);

MsgResult_t Set_433Channel_Attr(UnitPara_t *pData);
MsgResult_t Get_433Channel_Attr(UnitPara_t *pData, BF_INT08U *rlen, BF_INT08U *rpara);

#endif //DeviceUpgradeAttr.h
/**************************Copyright BestFu 2014-05-14*************************/

