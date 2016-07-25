/***************************Copyright xxxxx 2015-12-29*************************
**  ��	����system_stm32f10x.c
**  ˵	����ϵͳʱ�ӳ�ʼ�����Դ����
**  ��	�룺uVision V5.12.0.0
**  ��	����V1.0
**  ��	д��Jay
**  ��	�ڣ�2015/12/29
**  �ޡ��ģ�����
*******************************************************************************/
#include "BF_type.h"
#include "stm32f10x.h"
#include "DevPartTab.h"

//pravite macros define
#define VECT_TAB_START_ADDRESS  (DevPartTab->user_app_start_addr) 	//�ж����������ʼ��ַ
#define HSE_VALUE		 		((uint32_t)8000000)					//����ʹ�õ��ⲿ����Ĵ�С

//pravite variable define
__I uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};

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
	/* Reset the RCC clock configuration to the default reset state(for debug purpose) */
	/* Set HSION bit */
	RCC->CR |= (uint32_t)0x00000001;

	/* Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits */
	RCC->CFGR &= (uint32_t)0xF8FF0000; 

	/* Reset HSEON, CSSON and PLLON bits */
	RCC->CR &= (uint32_t)0xFEF6FFFF;

	/* Reset HSEBYP bit */
	RCC->CR &= (uint32_t)0xFFFBFFFF;

	/* Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE bits */
	RCC->CFGR &= (uint32_t)0xFF80FFFF;

	/* Disable all interrupts and clear pending bits  */
	RCC->CIR = 0x009F0000;

	/* Configure the System clock frequency, HCLK, PCLK2 and PCLK1 prescalers */
	/* Configure the Flash Latency cycles and enable prefetch buffer */
	SetSysClock();

#ifdef VECT_TAB_SRAM
	SCB->VTOR = VECT_TAB_START_ADDRESS; /* Vector Table Relocation in Internal SRAM. */
#else
	SCB->VTOR = VECT_TAB_START_ADDRESS;
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

	/* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/    
	/* Enable HSE */    
	RCC->CR |= ((uint32_t)RCC_CR_HSEON);

	/* Wait till HSE is ready and if Time out is reached exit */
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
		/* Enable Prefetch Buffer */
		FLASH->ACR |= FLASH_ACR_PRFTBE;

		/* Flash 2 wait state */
		FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
		FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_2;    

		/* HCLK = SYSCLK */
		RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

		/* PCLK2 = HCLK */
		RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;

		/* PCLK1 = HCLK */
		RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;

		/*  PLL configuration: PLLCLK = HSE * 9 = 72 MHz */
		RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE |
						RCC_CFGR_PLLMULL));
		RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMULL9);

		/* Enable PLL */
		RCC->CR |= RCC_CR_PLLON;

		/* Wait till PLL is ready */
		while((RCC->CR & RCC_CR_PLLRDY) == 0)
		{
		}

		/* Select PLL as system clock source */
		RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
		RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;    

		/* Wait till PLL is used as system clock source */
		while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08)
		{
		}
	}
	else
	{ 
	/*�������˵���ⲿʱ�ӳ���������*/	
	/*ʹ��HSI,16��Ƶ��Ϊ System Clock*/	
	/* ��Ƶ��� System Clock Ϊ64MHz*/

	/* Enable Prefetch Buffer */
	FLASH->ACR |= FLASH_ACR_PRFTBE;

	/* Flash 2 wait state */
	FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
	FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_2;    


	/* HCLK = SYSCLK */
	RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

	/* PCLK2 = HCLK */
	RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;

	/* PCLK1 = HCLK */
	RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;

	RCC->CR &= ~(1<<16);//�ر�HSE

	RCC->CR |= 0x00000001;//HSI����
	while(!RCC->CR & 0x00000002){};//�ȴ�HSI����

	RCC->CFGR &= ~(1<<16);//HSIʱ��2��Ƶ����ΪPLL����ʱ��

	RCC->CFGR |= 0x00380000;//PLL 16 ��Ƶ

	/* Enable PLL */
	RCC->CR |= RCC_CR_PLLON;

    /*�ȴ�PLL���� */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }
		
	/* ѡ��PLL��Ϊϵͳʱ��Դ */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;    

    /* �ȴ�PLL��Ϊϵͳʱ�� */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08)
    {
		
    }	
  }
}

/*******************************************************************************
** ��������: 	Get_SysClk_FrqAndSrc
** ��������: 	��ȡϵͳ��ʱ��Ƶ�ʺ�ʱ��Դ
** �䡡  ��:    u32 *frq��������ʱ��Ƶ��
				u8 *src ������ʱ��Դ 0���ڲ�ʱ��  1���ⲿʱ��
** ��    ��:   	��
*******************************************************************************/
u8  Get_SysClk_FrqAndSrc(u32 *frq,u8 *src)
{
	uint32_t tmp = 0, pllmull = 0, pllsource = 0;
	uint32_t SystemCoreClock = 0u;

	/* Get SYSCLK source -------------------------------------------------------*/
	tmp = RCC->CFGR & RCC_CFGR_SWS;
	switch (tmp)
	{
		case 0x00:  /* HSI used as system clock */
		{
			SystemCoreClock = HSI_VALUE;
			*src = 0;	
		}
		break;
		case 0x04:  /* HSE used as system clock */
		{
			SystemCoreClock = HSE_VALUE;
			*src = 1;
		}
		break;
		case 0x08:  /* PLL used as system clock */
		{
			/* Get PLL clock source and multiplication factor ----------------------*/
			pllmull = RCC->CFGR & RCC_CFGR_PLLMULL;
			pllsource = RCC->CFGR & RCC_CFGR_PLLSRC;
			if(pllsource)
			{
				*src = 1;
			}
			else
			{
				*src = 0;
			}         
			pllmull = ( pllmull >> 18) + 2;

			if (pllsource == 0x00)
			{
				/* HSI oscillator clock divided by 2 selected as PLL clock entry */
				SystemCoreClock = (HSI_VALUE >> 1) * pllmull;
			}
			else
			{
				/* HSE selected as PLL clock entry */
				if ((RCC->CFGR & RCC_CFGR_PLLXTPRE) != (uint32_t)RESET)
				{/* HSE oscillator clock divided by 2 */
					SystemCoreClock = (HSE_VALUE >> 1) * pllmull;
				}
				else
				{
					SystemCoreClock = HSE_VALUE * pllmull;
				}
			}
		}
		break;
		default:
			SystemCoreClock = HSI_VALUE;
		break;		
	}
	/* Compute HCLK clock frequency ----------------*/
	/* Get HCLK prescaler */
	tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4)];
	/* HCLK clock frequency */
	SystemCoreClock >>= tmp;  
	*frq = SystemCoreClock;////
	return 1;
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
