/***************************Copyright bestfu 2015-12-30**************************
**  文    件: gpio.c
**  功    能：stm32f10x GPIO驱动
**  编    译：Keil uVision5 V5.12
**  版    本：V1.0.0
**  编    写：Jay
**  创建日期：2015.12.29
**  修改日期：无
**  说    明：初版
*******************************************************************************/
#include "stm32f10x.h"
#include "lib_type.h"
#include "gpio.h"

//pravite macros define
#define EVCR_PORTPINCONFIG_MASK     ((BF_INT16U)0xFF80)
#define LSB_MASK                    ((BF_INT16U)0xFFFF)
#define DBGAFR_POSITION_MASK        ((BF_INT32U)0x000F0000)
#define DBGAFR_SWJCFG_MASK          ((BF_INT32U)0xF0FFFFFF)
#define DBGAFR_LOCATION_MASK        ((BF_INT32U)0x00200000)
#define DBGAFR_NUMBITS_MASK         ((BF_INT32U)0x00100000)

/*******************************************************************************
函 数 名：	void GPIO_Config(GPIO_Port GPIO_x,BF_INT16U GPIO_Pin,GPIO_Mode mode)
功能说明： 	配置GPIO引脚状态
参	  数： 	GPIOx   : 对应的GPIO_A~GPIO_G
			GPIO_Pin: 管脚GPIO_Pin_0~GPIO_Pin_15
			mode    :  作用(IN/OUT/AF/AN)_功能((IN_/ /UP|DOWN/FLOATING)
			(OUT/AF_(OP(开漏)/PP(推挽)))_速度(2M/25M/50M/100M)
			(详情请查阅头文件)
返 回 值：	无
说    明:   无
*******************************************************************************/
void GPIO_Config(GPIO_Port GPIO_x,BF_INT16U GPIO_Pin,GPIO_Mode mode)
{
    BF_INT32U currentmode = 0x00, pinpos = 0x00;
    volatile BF_INT32U tmpregtab[2] = {0};
	
	GPIO_TypeDef *GPIOx = (GPIO_TypeDef*)(GPIOA_BASE + (GPIO_x<<10)); 

    /*---------------------------配置GPIO时钟----------------------------------*/
    if(!(RCC->APB2ENR & ((1u) << (GPIO_x + 2))))
    {
        RCC->APB2ENR |= ((1u) << (GPIO_x + 2));
    }
    /*---------------------------- GPIO 模式配置 --------------------------------*/
    currentmode = ((BF_INT32U)mode)&((BF_INT32U)(0x0f));
    tmpregtab[0] = GPIOx->CRL;
    tmpregtab[1] = GPIOx->CRH;

    for (pinpos = 0; pinpos < 16; pinpos++)
    {
        if(0 != (((BF_INT32U)GPIO_Pin) & (1ul << pinpos)))
        {
            *(tmpregtab + pinpos/8) &=~(0x0f << (4 * (pinpos%8)));
            *(tmpregtab + pinpos/8) |= (currentmode << (4 * (pinpos%8)));
            if (mode == IN_UP)
            {
                GPIOx->BRR = (((BF_INT32U)0x01) << pinpos);
            }
            else
            {
                if (mode == IN_DOWN)
                {
                    GPIOx->BSRR = (((BF_INT32U)0x01) << pinpos);
                }
            }
        }
    }
    if (((BF_INT32U)GPIO_Pin & ((BF_INT32U)0x00FF)) != 0x00)
    {
        GPIOx->CRL = tmpregtab[0];
    }
    if (GPIO_Pin > 0x00FF)
    {
        GPIOx->CRH = tmpregtab[1];
    }
}

/*******************************************************************************
函 数 名：	void GPIO_SetBits(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
功能说明： 	设置引脚输出状态
参	  数： 	GPIOx   : 对应的GPIO_A~GPIO_G
			GPIO_Pin: 管脚GPIO_Pin_0~GPIO_Pin_15
返 回 值：	无
注    意:   GPIO_Pin = 0xffff; 时即对此端口的所有引脚进行置1
*******************************************************************************/
void GPIO_SetBits(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
{
    GPIO_TypeDef *GPIOx = (GPIO_TypeDef*)(GPIOA_BASE + (GPIO_x<<10)); 
	GPIOx->BSRR = GPIO_Pin;
}

/*******************************************************************************
函 数 名：	void GPIO_ResetBits(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
功能说明： 	复位引脚输出状态
参	  数： 	GPIOx   : 对应的GPIO_A~GPIO_G
			GPIO_Pin: 管脚GPIO_Pin_0~GPIO_Pin_15
返 回 值：	无
注    意:   当GPIO_Pin= 0xffff; 时即对此端口的所有引脚进行置0
*******************************************************************************/
void GPIO_ResetBits(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
{
    GPIO_TypeDef *GPIOx = (GPIO_TypeDef*)(GPIOA_BASE + (GPIO_x<<10)); 
	GPIOx->BRR = GPIO_Pin;
}

/*******************************************************************************
函 数 名：	void GPIO_ToggleBits(GPIO_Port GPIO_x, BF_INT16U pin)
功能说明： 	取反引脚输出状态(reverse)
参	  数： 	GPIOx   : 对应的GPIO_A~GPIO_G
			GPIO_Pin: 管脚GPIO_Pin_0~GPIO_Pin_15
返 回 值：	无
注    意:   当pin = 0xffff; 时即对此端口的所有引脚进行取反
*******************************************************************************/
void GPIO_ToggleBits(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
{
    GPIO_TypeDef *GPIOx = (GPIO_TypeDef*)(GPIOA_BASE + (GPIO_x<<10)); 
	GPIOx->ODR ^= GPIO_Pin;
}

/*******************************************************************************
函 数 名：	BF_INT08U GPIO_ReadOutputDataBit(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
功能说明： 	获取相应端口各引脚的输出状态值
参	  数： 	GPIOx: 对应的GPIO_(A~G)
			pin	 : 管脚pin_(0~15)
返 回 值：	返回无符号8位整数,若该引脚被设置则为1,否则为0;
注    意: 	无
*******************************************************************************/
BF_INT08U GPIO_ReadOutputDataBit(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
{
    BF_INT08U bitstatus = 0x00;
	GPIO_TypeDef *GPIOx = (GPIO_TypeDef*)(GPIOA_BASE + (GPIO_x<<10)); 
	
    if ((GPIOx->ODR & GPIO_Pin) != (BF_INT32U)RESET)
    {
        bitstatus = (BF_INT08U)SET;
    }
    else
    {
        bitstatus = (BF_INT08U)RESET;
    }
    return bitstatus;
}

/*******************************************************************************
函 数 名：	BF_INT08U GPIO_ReadInputDataBit(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
功能说明： 	获取相应端口各引脚的输入状态值
参	  数： 	GPIOx: 对应的GPIO_(A~G)
			pin	 : 管脚pin_(0~15)
返 回 值：	返回无符号8位整数,若该引脚被设置则为1,否则为0;
注    意: 	无
*******************************************************************************/
BF_INT08U GPIO_ReadInputDataBit(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
{
    BF_INT08U bitstatus = 0x00;
	GPIO_TypeDef *GPIOx = (GPIO_TypeDef*)(GPIOA_BASE + (GPIO_x<<10)); 

    if ((GPIOx->IDR & GPIO_Pin) != (BF_INT32U)RESET)
    {
        bitstatus = (BF_INT08U)SET;
    }
    else
    {
        bitstatus = (BF_INT08U)RESET;
    }
    return bitstatus;
}

/*******************************************************************************
函 数 名：	void GPIO_PinLockConfig(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
功能说明： 	锁定相应GPIO的相应管脚
参	  数： 	GPIOx: 对应的GPIO_(A~G)
			pin	 : 管脚pin_(0~15)
返 回 值：	无
注    意: 	在写序列期间,不能更改LCKR[15:0] 的值;将LOCK 序 列 应 用
			到某个端口位后,在执行下一次复位之前,将无法对该端口位的值进行修改;
*******************************************************************************/
void GPIO_PinLockConfig(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
{
    volatile BF_INT32U tmp = 0x00010000; //使能端口位锁定功能
	GPIO_TypeDef *GPIOx = (GPIO_TypeDef*)(GPIOA_BASE + (GPIO_x<<10)); 
	
    tmp |= GPIO_Pin;
    /* Set LCKK bit */
    GPIOx->LCKR = tmp;
    /* Reset LCKK bit */
    GPIOx->LCKR =  GPIO_Pin;
    /* Set LCKK bit */
    GPIOx->LCKR = tmp;
    /* Read LCKK bit*/
    tmp = GPIOx->LCKR;
    /* Read LCKK bit*/
    tmp = GPIOx->LCKR;
}

/*******************************************************************************
函 数 名：	void GPIO_PinRemapConfig(BF_INT32U GPIO_Remap, FunctionalState NewState)
功能说明： 	配置GPIO的重映射功能
参	  数： 	GPIO_Remap    : GPIO_Remap_SPI1 ~ GPIO_Remap_MISC (详情请参看头文件)
			NewState      : ENABLE表示使能,DISENABLE表示禁能
返 回 值：	无
注    意: 	无
*******************************************************************************/
void GPIO_PinRemapConfig(BF_INT32U GPIO_Remap, BF_INT08U NewState)
{
    BF_INT32U tmp = 0x00, tmp1 = 0x00, tmpreg = 0x00, tmpmask = 0x00;

    if((GPIO_Remap & 0x80000000) == 0x80000000)
    {
        tmpreg = AFIO->MAPR2;
    }
    else
    {
        tmpreg = AFIO->MAPR;
    }

    tmpmask = (GPIO_Remap & DBGAFR_POSITION_MASK) >> 0x10;
    tmp = GPIO_Remap & LSB_MASK;

    if ((GPIO_Remap & (DBGAFR_LOCATION_MASK | DBGAFR_NUMBITS_MASK)) == (DBGAFR_LOCATION_MASK | DBGAFR_NUMBITS_MASK))
    {
        tmpreg &= DBGAFR_SWJCFG_MASK;
        AFIO->MAPR &= DBGAFR_SWJCFG_MASK;
    }
    else if ((GPIO_Remap & DBGAFR_NUMBITS_MASK) == DBGAFR_NUMBITS_MASK)
    {
        tmp1 = ((BF_INT32U)0x03) << tmpmask;
        tmpreg &= ~tmp1;
        tmpreg |= ~DBGAFR_SWJCFG_MASK;
    }
    else
    {
        tmpreg &= ~(tmp << ((GPIO_Remap >> 0x15)*0x10));
        tmpreg |= ~DBGAFR_SWJCFG_MASK;
    }

    if (NewState != DISENABLE)
    {
        tmpreg |= (tmp << ((GPIO_Remap >> 0x15)*0x10));
    }

    if((GPIO_Remap & 0x80000000) == 0x80000000)
    {
        AFIO->MAPR2 = tmpreg;
    }
    else
    {
        AFIO->MAPR = tmpreg;
    }
}

/********************************End of File***********************************/
