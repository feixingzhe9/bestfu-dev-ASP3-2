/***************************Copyright BestFu 2014-05-14*************************
文	件：	BestFulib.h
说	明：	BestFu自建Lib库头文件
编	译：	Keil uVision4 V4.54.0.0
版	本：	v1.0
编	写：	Unarty
日	期：	2014-05-30
*******************************************************************************/
#ifndef __BESTFULIB_H
#define __BESTFULIB_H

/**********************************定义自定义宏*******************************/

/**********************************函数声明************************************/
void memcpy(void *dest,  const void *src, BF_INT32U num);
BF_BOOLEAN memcmp(void *dest,  const void *src, BF_INT32U num);
void memset(void *dest,const BF_INT08U val, BF_INT32U num);
void memmove(void *dest,  const void *src, BF_INT32U num);
void* memfind(void *dest, BF_INT32U dLen, void *src, BF_INT32U sLen);

#endif //BestFuLib.h end
/**************************Copyright BestFu 2014-05-14*************************/
							
