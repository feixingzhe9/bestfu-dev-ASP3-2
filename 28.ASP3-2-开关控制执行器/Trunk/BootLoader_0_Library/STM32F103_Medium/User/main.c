/***************************Copyright BestFu 2016-04-13*************************
文	件：	main.c
说	明：	系统主函数
编	译：	uVision V5.12.0
版	本：	v1.0
编	写：	jay
日	期：	2015-04-13 
修　改：	暂无
*******************************************************************************/
#include "main.h"

static void JumpToCode(unsigned int addr);
/*******************************************************************************
函数  名:  	main
功能说明:	主函数
参    数:  	无
返回  值:  	无
*******************************************************************************/
int main(void)
{
	Bootloader0Info_t* pboot0Info = (Bootloader0Info_t*)(DevPartTab->boot0_flag_start_addr);
	JumpToCode((BOOTLOADER_0_JUMP_FLAG == pboot0Info->bootloader_0_flag)?(DevPartTab->user_app_start_addr):(DevPartTab->boot1_start_addr));
	for(;;);
}

/*******************************************************************
函 数 名：	 __set_MSP
功能说明：	设置堆栈指针地址
参	  数：	无
返 回 值：	无
*******************************************************************/
__asm void __set_MSP(unsigned int mainStackPointer)
{
	msr msp, r0
	bx lr
}

/*******************************************************************
函 数 名：	Jump_To_Code
功能说明：	跳转到地址对应的程序区运行
参	  数：	addr:	对代码存储区首地址
返 回 值：	无
*******************************************************************/
static void JumpToCode(unsigned int addr)
{
	volatile unsigned int* p = (volatile unsigned int*)addr;
	__set_MSP(*p);		//取出初始的MSP的值
	((void (*)(void))(*(p + 1)))();
}
/**************************Copyright BestFu 2016-04-13*************************/
