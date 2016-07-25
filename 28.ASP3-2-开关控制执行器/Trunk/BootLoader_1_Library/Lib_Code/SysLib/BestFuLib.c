/***************************Copyright BestFu 2014-05-14*************************
文	件：	BestFulib.c
说	明：	BestFu自建Lib库文件
编	译：	Keil uVision4 V4.54.0.0
版	本：	v1.0
编	写：	Unarty
日	期：	2014-05-30
修  改:     2014-09-27 Unarty Add memfind && memcharget
*******************************************************************************/
#include "lib_type.h"
#include "lib_config.h"
#include "BestFuLib.h"

/*******************************************************************************
函 数 名：	memcpy
功能说明： 	内存考备
参	  数： 	dest:	目标内存地址
			src:	源内存地址
			num:	考备内存大小
返 回 值：	无
*******************************************************************************/
void memcpy(void *dest,  const void *src, BF_INT32U num)
{
    BF_CHAR *pd, *ps;

    if (dest != src)  	//目标与源不是同一位置
    {
        for (pd = (BF_CHAR*)dest, ps = (BF_CHAR*)src; num > 0; num--)
        {
            *pd++ = *ps++;
        }
    }
}

/*******************************************************************************
函 数 名：	memcmp
功能说明： 	内存比较
参	  数： 	dest:	目标内存地址
			src:	源内存地址
			num:	比较内存大小
返 回 值：	TRUE(相同)/FALSE
*******************************************************************************/
BF_BOOLEAN memcmp(void *dest,  const void *src, BF_INT32U num)
{
    BF_CHAR *pd, *ps;

    if (dest != src) 	//目标与源不是同一位置
    {
        for (pd = (BF_CHAR*)dest, ps = (BF_CHAR*)src; num > 0; num--)
        {
            if (*pd++ != *ps++)
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

/*******************************************************************************
函 数 名：	memset
功能说明： 	内存设置
参	  数： 	dest:	目标内存地址
			val:	目标值
			num:	比较内存大小
返 回 值：	无
*******************************************************************************/
void memset(void *dest,const BF_INT08U val, BF_INT32U num)
{
    BF_CHAR *pd;

    for (pd = (BF_CHAR*)dest; num > 0; num--)
    {
        *pd++ = val;
    }
}

/*******************************************************************************
函 数 名：	memmove
功能说明： 	内存移动
参	  数： 	dest:	目标内存地址
			src:	源内存地址
			num:	移动内存大小
返 回 值：	无
*******************************************************************************/
void memmove(void *dest,  const void *src, BF_INT32U num)
{
    BF_CHAR *pd, *ps;

    if (dest < src)
    {
        for (pd = (BF_CHAR*)dest, ps = (BF_CHAR*)src; num > 0; num--)
        {
            *pd++ = *ps++;
        }
    }
    else if (dest > src)
    {
        for (pd = (BF_CHAR*)((BF_INT32U)dest + num),\
                ps = (BF_CHAR*)((BF_INT32U)src + num); num > 0; num--)
        {
            *(--pd) = *(--ps);
        }
    }
}

/*******************************************************************************
函 数 名:  	memfind
功能说明:  	内存查找
参    数:  	dest:	查找目标
			dLen：	目标有效长度
			src:	查找源
			sLen:	源数据大小
返 回 值:  	NULL(未找到)/目标位于源的地址
*******************************************************************************/
void* memfind(void *dest, BF_INT32U dLen, void *src, BF_INT32U sLen)
{
    BF_CHAR *pDest, *pSrc;
    BF_INT32U i;

    for (pDest = dest, pSrc = src; sLen > 0; sLen--, pSrc++)
    {
        if (*pDest == *pSrc++) 									//源中找到与目标相同数据
        {
            for (i = 1; i < dLen && pDest[i] == *pSrc++; i++);	//比较后面数据是否相等
            if (i == dLen) 										//源中找到与与目标完全相等的数据片
            {
                return (pSrc - i);
            }
        }
    }
    return (NULL);
}
/**************************Copyright BestFu 2014-05-14*************************/
