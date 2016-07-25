/***************************Copyright BestFu 2014-05-14*************************
文	件：	Object.c
说	明：	目标数据匹配相关函数
编	译：	Keil uVision4 V4.54.0.0
版	本：	v1.0
编	写：	Unarty
日	期：	2014.06.23
修　改：	暂无
*******************************************************************************/
#include "lib_type.h"
#include "fifo.h"
#include "PivotalDataType.h"
#include "UserData.h"
#include "object.h"


//public variable define
BF_INT32U DelayID;	//用于通信匹配

//Private variable define
static RepeatSt RData[REPEAT_MAX]= {0}; //周旺增加重复流水号，增加时间判断。2015.7.30

//Private function declaration
static BF_INT08U Check_Repeat(const CMDAim_t *aim);    //消息重复核对

/*******************************************************************************
函 数 名：	Object_Check
功能说明： 	目标核对
参	  数： 	aim:		消息目标内容
			*validUnit:	目标操作有效单元
返 回 值：	0(消息匹配成功）/ 1(用户号不同）/2(指令重复)/3 (目标不匹配）
            /4(区域不匹配)/5(消息类型越界)
*******************************************************************************/
MsgResult_t Object_Check(BF_INT08U *sdata, BF_INT08U len, CMDAim_t *aim, CMDPara_t *para, BF_INT08U *data)
{
    if ( aim->userID == gSysData.userID				//校验主板用户名
            || aim->userID == SUPER_USERID			//校验超级用户号
            || gSysData.userID == DEFAULT_USERID	//用户号为默认值
       )
    {
        if (Check_Repeat(aim) == FALSE)  			//指令重复
        {
            return MSG_REPEAT;						//返回指令已运行
        }
        if (aim->objectType == SINGLE_ACT)			//单节点操作
        {
            if (aim->object.id != gSysData.deviceID)
            {
                return ID_ERR;						//返回目标匹配错误
            }
            DelayID = 0;							//用于通信匹配（Unarty 2014.08.07)
        }
        else if (aim->objectType == DEV_ALL_ACT && aim->object.id == 0)
        {
            DelayID = aim->sourceID;				//用于通信匹配（Unarty 2014.08.07)
        }
		else
		{
			return NO_CMD;
		}
		
		aim->object.id = aim->sourceID;      		//交换地址
        return COMPLETE;
    }

    return USER_ID_ERR;
}


/*******************************************************************************
函 数 名：	Check_Repeat
功能说明： 	核对指令是否重复
参	  数： 	data:	要核对的指令内容
返 回 值：	TRUE(重复)/FALSE(不重复)
*******************************************************************************/
static BF_INT08U Check_Repeat(const CMDAim_t *aim)
{
    static BF_INT08U seat;
    BF_INT08U i,j;

    for (i = 0; i < REPEAT_MAX; i++)
    {
        if (RData[i].srcID == aim->sourceID)  	//数据源地址相等
        {
            for(j=0; j < 5; j++)
            {
                if(RData[i].serial[j] == aim->serialNum)
                {
                    return FALSE;	//数据已处理
                }
            }

            RData[i].serialSeat = ++RData[i].serialSeat%5;   //流水号偏移位置
            RData[i].serial[RData[i].serialSeat] = aim->serialNum;	//记录流水号
            return TRUE;
        }
    }
    //原地址不相同
    seat = ++seat%REPEAT_MAX; //原地址偏移位置
    RData[seat].serial[0] = aim->serialNum; //流水号
    RData[seat].srcID     = aim->sourceID;  //原地址

    return TRUE;
}
/**************************Copyright BestFu 2014-05-14*************************/
