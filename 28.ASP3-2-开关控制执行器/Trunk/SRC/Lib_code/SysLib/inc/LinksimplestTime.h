/***************************Copyright BestFu 2014-05-14*************************
文	件：	LinksimplestTime.h
说	明：	最简联动带延时源文件
编	译：	Keil uVision4 V5.12.0.0
版	本：	v1.0
编	写：	Joey
日	期：	2016.1.29 
修　改：	暂无
*******************************************************************************/
#ifndef _LinkSimplestTime_
#define _LinkSimplestTime_

#include "linkall.h"

#if(ALINKTIME_EN > 0)
//typedef struct //延时时间结构体,5Bytes
//{
//	u8   delaytime[2];  //当前延时时间
//	u8   keeptime[2];   //当前保持时间
//	u8   repeat;        //该条消息重复次数
//}DlyTimeDat_t;

typedef struct //简单联动结构,32Bytes
{
	u8   UseFlag;      //使用标志
	u8   LinkNum;      //联动号
	DlyTimeData_t  Time;
}ALinkTimeItem;

typedef struct
{
	ALinkTimeItem   Item[MAX_ALINKTIMEITEM];  //联动项
	u8       Cnt;           //计数
}ALinkTimeTable;

__inline u32 GetALinkTimeInited(void)
{
	return ALINK_TIME_START_ADDR;
}

__inline u32 GetALinkTimeStartAddr(void)
{
	return ALINK_TIME_START_ADDR + 1;
}

void ALinkTimeInit(void);
void AllALinkTimeClr(void);
u8 QueryALinkTime(u8 num);
u8 DelALinkTime(u8 num);
u8 WriteALinkTime(u8 num, u8 linknum, u8 addflag, u8 *data);
u8 ReadALinkTime(u8 num , u8 *len, u8 *data);
u8 WriteALinkTimeToTable(u8 num, u8 linknum, u8 addflag, u8 *data);
u8 DelALinkTimeFromTable(u8 num);
u8 AddALinkTimeCmdX1(u8 num, u8 *data);

#endif
#endif
/**************************Copyright BestFu 2014-05-14*************************/
