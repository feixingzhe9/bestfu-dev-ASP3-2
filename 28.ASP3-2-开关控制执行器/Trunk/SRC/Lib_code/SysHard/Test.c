/***************************Copyright BestFu 2014-05-14*************************
��	����	Test.c
˵	����	���Ժ���
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2014.07.03 
�ޡ��ģ�	����
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
�� �� ����	Test_Init
����˵���� 	���ʼ������
��	  ���� 	��
�� �� ֵ��	��
*******************************************************************************/
void Test_Init(void)
{
	TEST_KEY_INIT;
	
	if (!TEST_KEY_VAL)
	{
		delay_ms(10);
		if (!TEST_KEY_VAL) //������밴��
		{
			Test = TEST_STATE;
			UART_Init(72, 9600);
			ProcessSend_Fun(&UART_Send); //�л��źŴ�����
			Si4432_Idle();;	//������ģ�����ڷ���״̬��ʹ�䲻���������ⲿ�ź�
			return;
		}
	}
	
	Test = 0;
}

/**************************Copyright BestFu 2014-05-14*************************/
