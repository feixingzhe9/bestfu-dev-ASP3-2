/***************************Copyright BestFu 2014-05-14*************************
文	件：    PivotalDataType.c
说	明:	    库关键数据类型
编	译：    uVision V5.12.0
版	本：    v1.0
编	写：    jay
日	期：    2016-04-14
修  改:     暂无
*******************************************************************************/
#include "lib_type.h"
#include "lib_config.h"
#include "PivotalDataType.h"

//private variable define
const BF_INT08U HeadCMD[HEAD_SIZE] = {0xF1, 0xF1, 0xF1, 0xF2, 0xF1, 0xF1};
const BF_INT08U TailCMD[TAIL_SIZE] = {0xF2, 0xF2, 0xF2, 0xF1, 0xF2, 0xF2};


/**************************Copyright BestFu 2014-05-14*************************/
