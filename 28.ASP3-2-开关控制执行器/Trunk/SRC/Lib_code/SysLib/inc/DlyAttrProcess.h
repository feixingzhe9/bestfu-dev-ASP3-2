/***************************Copyright BestFu 2016-01-28*************************
文	件：    DlyAttrProcess.h
说	明：    系统属性顺序执行处理函数头文件
编	译：    Keil uVision V5.12.0.0
版	本：    v1.0
编	写：    jay
日	期：    2016-01-28
修  改:     无
*******************************************************************************/
#ifndef __DLYATTRPROCESS_H
#define __DLYATTRPROCESS_H

#include "BF_type.h"
#include "UnitCfg.h"
#include "linkcommon.h"
#include "UserData.h"

/*****************************关键宏配置**************************************/
#if (SCENCETIME_EN > 0u)
	#define  DALAY_EXECUTE_EN	(1u)			//配置执行功能
#else
	#define  DALAY_EXECUTE_EN	(0u)			//配置执行功能
#endif

#define  TOTAL_Block_NUM        (32)			//内存块的数量
#define  TRAVERCYCLE  			(50)			//延时任务的遍历周期

#define  DELAY_PRECISION        (100u)    		//延时精度100ms
#define  KEEP_PRECISION         (100u)     		//延时精度100ms

#define  UNIT_PROPERTY_RELATE	((u8)0xFF)		//全单元属性关联


/*******************************定义数据结构**********************************/
#if ((ALINKTIME_EN > 0u) || (SCENCETIME_EN > 0u))
#pragma pack(1)
typedef struct									//延时时间结构体,5Bytes
{
	u8 delaytime[2];							//当前延时时间
	u8 keeptime[2]; 							//当前保持时间
	u8 repeat;		   							//该条消息重复次数
}DlyTimeData_t;

#pragma pack()
#endif

#if (DALAY_EXECUTE_EN > 0u)
#pragma pack(1)
typedef struct									//延时执行数据域数据结构
{
	u8 unit;									//单元号
	u8 Cmd;  									//命令号
	u8 len;  									//参数长度
	u8 data[13];								//参数区数据
	u8 remaindelaytime[4];						//剩余延时时间值
	u8 remainkeeptime[4]; 						//剩余保持时间值
	u8 delaytaskexeflag;						//延时任务执行标记
	u8 keeptaskexeflag;							//保持任务执行标记
	DlyTimeData_t DlyTimeData;		   			//延时时间结构体
	u8 unitAttrRelate;							//单个单元中属性之间的关联关系,注意0作为保留,没有关联意义
}DlyExeData_t;

typedef struct									//延时执行缓冲结构
{
	DlyExeData_t DlyExeData[TOTAL_Block_NUM];	//延时执行的数据
	u8 cnt;										//表示当前有效节点个数
}DlyExeNode_t;

#pragma pack()

typedef enum									//延时消息执行属性覆盖类型
{
	SAME_MSG_DEL = 0,							//相同单元属性消息覆盖
	RELATE_MSG_DEL								//相同关联属性消息覆盖
}DlyMsgDelType_t;

/**********************************函数声明**********************************/
u8 DlyMsgFilterProcess(UnitPara_t* para,DlyMsgDelType_t active);
u8 CreateBufferNode(u8 *data);
u8 DeleteDlyAssMsg(u8 unit,u8 Cmd);

#endif
#endif 
/******************************End of File************************************/
