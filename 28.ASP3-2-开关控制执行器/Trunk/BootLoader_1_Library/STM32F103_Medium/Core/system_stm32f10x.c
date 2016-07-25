/***************************Copyright xxxxx 2015-12-29*************************
**  文	件：system_stm32f10x.c
**  说	明：系统时钟初始化相关源代码
**  编	译：uVision V5.12.0.0
**  版	本：V1.0
**  编	写：Jay
**  日	期：2015/12/29
**  修　改：暂无
*******************************************************************************/
#include "stm32f10x.h"
#include "system_stm32f10x.h"
#include "lib_type.h"
#include "DevPartTab.h"

//pravite macros define
#define VECT_TAB_START_ADDRESS  (DevPartTab->boot1_start_addr) 	//中断向量表的偏移量
#define HSE_VALUE		 		((uint32_t)8000000)				//定义使用的外部晶振的大小

//pravite variable define
static __I uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
static __I uint8_t APBPrescTable[8] = {0, 0, 0, 0, 1, 2, 3, 4};
static __I uint8_t ADCPrescTable[4] = {2, 4, 6, 8};

//pravite function declaration
static void SetSysClock(void);
/*******************************************************************************
函 数 名:  	void SystemInit (void)
功能说明:  	系统时钟初始化
参    数:  	无
返 回 值:  	无
说    明:	若有外部sram必须初始化
*******************************************************************************/
void SystemInit (void)
{
    /* 开启内部时钟 */
    RCC->CR |= (uint32_t)0x00000001;

    /* 复位 SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO 位 */
    RCC->CFGR &= (uint32_t)0xF8FF0000;

    /* 复位 HSEON, CSSON and PLLON 位 */
    RCC->CR &= (uint32_t)0xFEF6FFFF;

    /* 复位 HSEBYP 位 */
    RCC->CR &= (uint32_t)0xFFFBFFFF;

    /* 复位 PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE 位 */
    RCC->CFGR &= (uint32_t)0xFF80FFFF;

    /* 关闭所有中断清除所有挂起标志位  */
    RCC->CIR = 0x009F0000;

    /* 配置系统时钟、HCLK,PCLK1、PCLK2预分频系数 */
    SetSysClock();

#ifdef VECT_TAB_SRAM
    SCB->VTOR = VECT_TAB_START_ADDRESS;  /* 重定向向量表在SRAM中的位置 */
#else
    SCB->VTOR = VECT_TAB_START_ADDRESS;  /* 重定向向量表在FLASH中的位置 */
#endif
}

/*******************************************************************************
函 数 名:  	static void SetSysClock(void)
功能说明:  	设置时钟参数
参    数:  	无
返 回 值:  	无
说    明:	无
*******************************************************************************/
static void SetSysClock(void)
{
    __IO uint32_t StartUpCounter = 0, HSEStatus = 0;

    /* 使能内部时钟 */
    RCC->CR |= ((uint32_t)RCC_CR_HSEON);

    /* 等待HSE时钟准备就绪，超时则退出 */
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
        /* 使能FLASH预取缓存 */
        FLASH->ACR |= FLASH_ACR_PRFTBE;

        FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
        FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_2;

        /* HCLK = SYSCLK */
        RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

        /* PCLK2 = HCLK */
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;

        /* PCLK1 = HCLK/2 */
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;

        /*  PLL 配置: PLLCLK = HSE * 9 = 72 MHz */
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE |
                                            RCC_CFGR_PLLMULL));
        RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMULL9);

        /* 使能 PLL */
        RCC->CR |= RCC_CR_PLLON;

        /* 等待PLL就绪 */
        while((RCC->CR & RCC_CR_PLLRDY) == 0)
        {
        }
        /* 选择PLL作为系统时钟资源 */
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
        RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

        /* 等待时钟切换时钟就绪*/
        while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08)
        {
        }
    }
    else
    {
    }
}

/*******************************************************************************
函 数 名:  	void RCC_GetClocksFreq(RCC_ClocksTypeDef* RCC_Clocks)
功能说明:  	获取运行系统的各种时钟
参    数:  	RCC_Clocks：时钟集合的结构体指针
返 回 值:  	无
说    明:	无
*******************************************************************************/
void RCC_GetClocksFreq(RCC_ClocksTypeDef* RCC_Clocks)
{
    uint32_t tmp = 0, pllmull = 0, pllsource = 0, presc = 0;

    /* 获取 SYSCLK 资源 -------------------------------------------------------*/
    tmp = RCC->CFGR & ((uint32_t)RCC_CFGR_SWS);

    switch (tmp)
    {
		case RCC_CFGR_SWS_HSE:  /* HSE 作为系统时钟资源 */
			RCC_Clocks->SYSCLK_Frequency = HSE_VALUE;
			break;
		case RCC_CFGR_SWS_PLL: /* PLL 作为系统时钟资源 */

			/* 获取PLL时钟源和倍频因子 ----------------------*/
			pllmull = RCC->CFGR & ((uint32_t)RCC_CFGR_PLLMULL);
			pllsource = RCC->CFGR & ((uint32_t)RCC_CFGR_PLLSRC);

			pllmull = ( pllmull >> 18) + 2;

			if (pllsource == 0x00)
			{   /* HSI二分频后作为PLL时钟输入 */
				RCC_Clocks->SYSCLK_Frequency = (HSI_VALUE >> 1) * pllmull;
			}
			else
			{
				/* HSE 作为PLL时钟输入 */
				if ((RCC->CFGR & ((uint32_t)RCC_CFGR_PLLXTPRE)) != (uint32_t)RESET)
				{   /* HSE/2 作为PLL时钟输入 */
					RCC_Clocks->SYSCLK_Frequency = (HSE_VALUE >> 1) * pllmull;
				}
				else
				{
					RCC_Clocks->SYSCLK_Frequency = HSE_VALUE * pllmull;
				}
			}
			break;
		case RCC_CFGR_SWS_HSI:	/* HSI 作为系统时钟资源 */

		default:
			RCC_Clocks->SYSCLK_Frequency = HSI_VALUE;
			break;
    }

    /* 捕获 HCLK, PCLK1, PCLK2 and ADCCLK clocks 频率 ----------------*/
    /* 获取 HCLK 预分频 */
    tmp = RCC->CFGR & ((uint32_t)RCC_CFGR_HPRE);
    tmp = tmp >> 4;
    presc = AHBPrescTable[tmp];
    /* HCLK 时钟频率 */
    RCC_Clocks->HCLK_Frequency = RCC_Clocks->SYSCLK_Frequency >> presc;
    /* 获取 PCLK1 预分频值 */
    tmp = RCC->CFGR & ((uint32_t)RCC_CFGR_PPRE1);
    tmp = tmp >> 8;
    presc = APBPrescTable[tmp];
    /* PCLK1 时钟频率 */
    RCC_Clocks->PCLK1_Frequency = RCC_Clocks->HCLK_Frequency >> presc;
    /* 获取 PCLK2 预分频值 */
    tmp = RCC->CFGR & ((uint32_t)RCC_CFGR_PPRE2);
    tmp = tmp >> 11;
    presc = APBPrescTable[tmp];
    /* PCLK2 时钟频率 */
    RCC_Clocks->PCLK2_Frequency = RCC_Clocks->HCLK_Frequency >> presc;
    /* 获取ADCCLK 预分频值 */
    tmp = RCC->CFGR & ((uint32_t)RCC_CFGR_ADCPRE);
    tmp = tmp >> 14;
    presc = ADCPrescTable[tmp];
    /* ADCCLK 时钟频率 */
    RCC_Clocks->ADCCLK_Frequency = RCC_Clocks->PCLK2_Frequency / presc;
}
/**********************************End of File*********************************/
