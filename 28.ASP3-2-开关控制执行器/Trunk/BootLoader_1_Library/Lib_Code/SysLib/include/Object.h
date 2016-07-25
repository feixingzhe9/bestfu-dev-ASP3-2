/***************************Copyright BestFu 2014-05-14*************************
文	件：	Object.h
说	明：	目标数据匹配头文件
编	译：	Keil uVision4 V4.54.0.0
版	本：	v1.0
编	写：	Unarty
日	期：	2014.06.23 
修　改：	暂无
*******************************************************************************/
#ifndef __OBJECT_H
#define __OBJECT_H

/**********************************定义自定义宏*******************************/
#define AREA_ALL    (0xFFFFFFFF)    //全区域
#define AREA_1      (0x00FFFFFF)    //区域等级一
#define AREA_2      (0x0000FFFF)    //区域等级二
#define AERA_3      (0x000000FF)    //区域等级三
#define AERA_4      (0x00000000)    //区域等级四
#define AREA_MAX	(5)				//最大区域数
#define AREA_VALUE  (0xFF)          //化分区域默认值

/********************************数据结构定义**********************************/
//周旺修改重复流水号，增加时间判断。2015.7.30
#define REPEAT_MAX (20)       //记录10个数据的流水号
#define REPEAT_OVERT (10000)  //超时时间为10秒
typedef struct 
{
	BF_INT32U  srcID;//判断重复源地址
	BF_INT32U  time;//当时时间
	BF_INT08U  serial[5];//流水号
	BF_INT08U  serialSeat;//流水号位置
}RepeatSt;

/************************************变量声明**********************************/
extern BF_INT32U DelayID;		//用于通信匹配（Unarty 2014.08.07)

/**********************************函数声明***********************************/
MsgResult_t Object_Check(BF_INT08U *sdata, BF_INT08U len, CMDAim_t *aim, CMDPara_t *para, BF_INT08U *data);

#endif //Object.h end
/**************************Copyright BestFu 2014-05-14*************************/
