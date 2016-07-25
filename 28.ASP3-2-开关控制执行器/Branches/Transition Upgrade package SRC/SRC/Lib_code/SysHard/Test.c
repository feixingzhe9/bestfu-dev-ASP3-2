/***************************Copyright BestFu 2014-05-14*************************
文	件：	Test.c
说	明：	测试函数
编	译：	Keil uVision4 V4.54.0.0
版	本：	v1.0
编	写：	Unarty
日	期：	2014.07.03 
修　改：	暂无
*******************************************************************************/
#include "BF_type.h"
#include "Test.h"
#include "cmdProcess.h"
#include "USART.h"
#include "SysHard.h"
#include "GPIO.h"
#include "delay.h"
#include "SI4432.h"

u32 Test;

/*******************************************************************************
函 数 名：	Test_Init
功能说明： 	测初始化函数
参	  数： 	无
返 回 值：	无
*******************************************************************************/
void Test_Init(void)
{
	TEST_KEY_INIT;
	
	if (!TEST_KEY_VAL)
	{
		delay_ms(10);
		if (!TEST_KEY_VAL) //检测输入按键
		{
			Test = TEST_STATE;
			UART_Init(72, 9600);
			ProcessSend_Fun(&UART_Send); //切换信号处理函数
			Si4432_Idle();;	//将无线模块置于发送状态，使其不接收其他外部信号
			return;
		}
	}
	
	Test = 0;
}

/**************************Copyright BestFu 2014-05-14*************************/
