/***************************Copyright xxxxx 2015-12-29*************************
**  ��	����system_stm32f10x.c
**  ˵	����ϵͳʱ�ӳ�ʼ�����Դ����
**  ��	�룺uVision V5.12.0.0
**  ��	����V1.0
**  ��	д��Jay
**  ��	�ڣ�2015/12/29
**  �ޡ��ģ�����
*******************************************************************************/
#include "stm32f10x.h"
#include "system_stm32f10x.h"
#include "lib_type.h"
#include "DevPartTab.h"

//pravite macros define
#define VECT_TAB_START_ADDRESS  (DevPartTab->boot1_start_addr) 	//�ж��������ƫ����
#define HSE_VALUE		 		((uint32_t)8000000)				//����ʹ�õ��ⲿ����Ĵ�С

//pravite variable define
static __I uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
static __I uint8_t APBPrescTable[8] = {0, 0, 0, 0, 1, 2, 3, 4};
static __I uint8_t ADCPrescTable[4] = {2, 4, 6, 8};

//pravite function declaration
static void SetSysClock(void);
/*******************************************************************************
�� �� ��:  	void SystemInit (void)
����˵��:  	ϵͳʱ�ӳ�ʼ��
��    ��:  	��
�� �� ֵ:  	��
˵    ��:	�����ⲿsram�����ʼ��
*******************************************************************************/
void SystemInit (void)
{
    /* �����ڲ�ʱ�� */
    RCC->CR |= (uint32_t)0x00000001;

    /* ��λ SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO λ */
    RCC->CFGR &= (uint32_t)0xF8FF0000;

    /* ��λ HSEON, CSSON and PLLON λ */
    RCC->CR &= (uint32_t)0xFEF6FFFF;

    /* ��λ HSEBYP λ */
    RCC->CR &= (uint32_t)0xFFFBFFFF;

    /* ��λ PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE λ */
    RCC->CFGR &= (uint32_t)0xFF80FFFF;

    /* �ر������ж�������й����־λ  */
    RCC->CIR = 0x009F0000;

    /* ����ϵͳʱ�ӡ�HCLK,PCLK1��PCLK2Ԥ��Ƶϵ�� */
    SetSysClock();

#ifdef VECT_TAB_SRAM
    SCB->VTOR = VECT_TAB_START_ADDRESS;  /* �ض�����������SRAM�е�λ�� */
#else
    SCB->VTOR = VECT_TAB_START_ADDRESS;  /* �ض�����������FLASH�е�λ�� */
#endif
}

/*******************************************************************************
�� �� ��:  	static void SetSysClock(void)
����˵��:  	����ʱ�Ӳ���
��    ��:  	��
�� �� ֵ:  	��
˵    ��:	��
*******************************************************************************/
static void SetSysClock(void)
{
    __IO uint32_t StartUpCounter = 0, HSEStatus = 0;

    /* ʹ���ڲ�ʱ�� */
    RCC->CR |= ((uint32_t)RCC_CR_HSEON);

    /* �ȴ�HSEʱ��׼����������ʱ���˳� */
    do
    {
        HSEStatus = RCC->CR & RCC_CR_HSERDY;
        StartUpCounter++;
    } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

    if ((RCC->CR & RCC_CR_HSERDY) != RESET)
    {
        HSEStatus = (uint32_t)0x01;
    }
    else
    {
        HSEStatus = (uint32_t)0x00;
    }

    if (HSEStatus == (uint32_t)0x01)
    {
        /* ʹ��FLASHԤȡ���� */
        FLASH->ACR |= FLASH_ACR_PRFTBE;

        FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
        FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_2;

        /* HCLK = SYSCLK */
        RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

        /* PCLK2 = HCLK */
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;

        /* PCLK1 = HCLK/2 */
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;

        /*  PLL ����: PLLCLK = HSE * 9 = 72 MHz */
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE |
                                            RCC_CFGR_PLLMULL));
        RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMULL9);

        /* ʹ�� PLL */
        RCC->CR |= RCC_CR_PLLON;

        /* �ȴ�PLL���� */
        while((RCC->CR & RCC_CR_PLLRDY) == 0)
        {
        }
        /* ѡ��PLL��Ϊϵͳʱ����Դ */
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
        RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

        /* �ȴ�ʱ���л�ʱ�Ӿ���*/
        while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08)
        {
        }
    }
    else
    {
    }
}

/*******************************************************************************
�� �� ��:  	void RCC_GetClocksFreq(RCC_ClocksTypeDef* RCC_Clocks)
����˵��:  	��ȡ����ϵͳ�ĸ���ʱ��
��    ��:  	RCC_Clocks��ʱ�Ӽ��ϵĽṹ��ָ��
�� �� ֵ:  	��
˵    ��:	��
*******************************************************************************/
void RCC_GetClocksFreq(RCC_ClocksTypeDef* RCC_Clocks)
{
    uint32_t tmp = 0, pllmull = 0, pllsource = 0, presc = 0;

    /* ��ȡ SYSCLK ��Դ -------------------------------------------------------*/
    tmp = RCC->CFGR & ((uint32_t)RCC_CFGR_SWS);

    switch (tmp)
    {
		case RCC_CFGR_SWS_HSE:  /* HSE ��Ϊϵͳʱ����Դ */
			RCC_Clocks->SYSCLK_Frequency = HSE_VALUE;
			break;
		case RCC_CFGR_SWS_PLL: /* PLL ��Ϊϵͳʱ����Դ */

			/* ��ȡPLLʱ��Դ�ͱ�Ƶ���� ----------------------*/
			pllmull = RCC->CFGR & ((uint32_t)RCC_CFGR_PLLMULL);
			pllsource = RCC->CFGR & ((uint32_t)RCC_CFGR_PLLSRC);

			pllmull = ( pllmull >> 18) + 2;

			if (pllsource == 0x00)
			{   /* HSI����Ƶ����ΪPLLʱ������ */
				RCC_Clocks->SYSCLK_Frequency = (HSI_VALUE >> 1) * pllmull;
			}
			else
			{
				/* HSE ��ΪPLLʱ������ */
				if ((RCC->CFGR & ((uint32_t)RCC_CFGR_PLLXTPRE)) != (uint32_t)RESET)
				{   /* HSE/2 ��ΪPLLʱ������ */
					RCC_Clocks->SYSCLK_Frequency = (HSE_VALUE >> 1) * pllmull;
				}
				else
				{
					RCC_Clocks->SYSCLK_Frequency = HSE_VALUE * pllmull;
				}
			}
			break;
		case RCC_CFGR_SWS_HSI:	/* HSI ��Ϊϵͳʱ����Դ */

		default:
			RCC_Clocks->SYSCLK_Frequency = HSI_VALUE;
			break;
    }

    /* ���� HCLK, PCLK1, PCLK2 and ADCCLK clocks Ƶ�� ----------------*/
    /* ��ȡ HCLK Ԥ��Ƶ */
    tmp = RCC->CFGR & ((uint32_t)RCC_CFGR_HPRE);
    tmp = tmp >> 4;
    presc = AHBPrescTable[tmp];
    /* HCLK ʱ��Ƶ�� */
    RCC_Clocks->HCLK_Frequency = RCC_Clocks->SYSCLK_Frequency >> presc;
    /* ��ȡ PCLK1 Ԥ��Ƶֵ */
    tmp = RCC->CFGR & ((uint32_t)RCC_CFGR_PPRE1);
    tmp = tmp >> 8;
    presc = APBPrescTable[tmp];
    /* PCLK1 ʱ��Ƶ�� */
    RCC_Clocks->PCLK1_Frequency = RCC_Clocks->HCLK_Frequency >> presc;
    /* ��ȡ PCLK2 Ԥ��Ƶֵ */
    tmp = RCC->CFGR & ((uint32_t)RCC_CFGR_PPRE2);
    tmp = tmp >> 11;
    presc = APBPrescTable[tmp];
    /* PCLK2 ʱ��Ƶ�� */
    RCC_Clocks->PCLK2_Frequency = RCC_Clocks->HCLK_Frequency >> presc;
    /* ��ȡADCCLK Ԥ��Ƶֵ */
    tmp = RCC->CFGR & ((uint32_t)RCC_CFGR_ADCPRE);
    tmp = tmp >> 14;
    presc = ADCPrescTable[tmp];
    /* ADCCLK ʱ��Ƶ�� */
    RCC_Clocks->ADCCLK_Frequency = RCC_Clocks->PCLK2_Frequency / presc;
}
/**********************************End of File*********************************/
