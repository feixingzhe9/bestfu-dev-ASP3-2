/***************************Copyright bestfu 2015-12-30**************************
**  ��    ��: gpio.c
**  ��    �ܣ�stm32f10x GPIO����
**  ��    �룺Keil uVision5 V5.12
**  ��    ����V1.0.0
**  ��    д��Jay
**  �������ڣ�2015.12.29
**  �޸����ڣ���
**  ˵    ��������
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
�� �� ����	void GPIO_Config(GPIO_Port GPIO_x,BF_INT16U GPIO_Pin,GPIO_Mode mode)
����˵���� 	����GPIO����״̬
��	  ���� 	GPIOx   : ��Ӧ��GPIO_A~GPIO_G
			GPIO_Pin: �ܽ�GPIO_Pin_0~GPIO_Pin_15
			mode    :  ����(IN/OUT/AF/AN)_����((IN_/ /UP|DOWN/FLOATING)
			(OUT/AF_(OP(��©)/PP(����)))_�ٶ�(2M/25M/50M/100M)
			(���������ͷ�ļ�)
�� �� ֵ��	��
˵    ��:   ��
*******************************************************************************/
void GPIO_Config(GPIO_Port GPIO_x,BF_INT16U GPIO_Pin,GPIO_Mode mode)
{
    BF_INT32U currentmode = 0x00, pinpos = 0x00;
    volatile BF_INT32U tmpregtab[2] = {0};
	
	GPIO_TypeDef *GPIOx = (GPIO_TypeDef*)(GPIOA_BASE + (GPIO_x<<10)); 

    /*---------------------------����GPIOʱ��----------------------------------*/
    if(!(RCC->APB2ENR & ((1u) << (GPIO_x + 2))))
    {
        RCC->APB2ENR |= ((1u) << (GPIO_x + 2));
    }
    /*---------------------------- GPIO ģʽ���� --------------------------------*/
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
�� �� ����	void GPIO_SetBits(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
����˵���� 	�����������״̬
��	  ���� 	GPIOx   : ��Ӧ��GPIO_A~GPIO_G
			GPIO_Pin: �ܽ�GPIO_Pin_0~GPIO_Pin_15
�� �� ֵ��	��
ע    ��:   GPIO_Pin = 0xffff; ʱ���Դ˶˿ڵ��������Ž�����1
*******************************************************************************/
void GPIO_SetBits(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
{
    GPIO_TypeDef *GPIOx = (GPIO_TypeDef*)(GPIOA_BASE + (GPIO_x<<10)); 
	GPIOx->BSRR = GPIO_Pin;
}

/*******************************************************************************
�� �� ����	void GPIO_ResetBits(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
����˵���� 	��λ�������״̬
��	  ���� 	GPIOx   : ��Ӧ��GPIO_A~GPIO_G
			GPIO_Pin: �ܽ�GPIO_Pin_0~GPIO_Pin_15
�� �� ֵ��	��
ע    ��:   ��GPIO_Pin= 0xffff; ʱ���Դ˶˿ڵ��������Ž�����0
*******************************************************************************/
void GPIO_ResetBits(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
{
    GPIO_TypeDef *GPIOx = (GPIO_TypeDef*)(GPIOA_BASE + (GPIO_x<<10)); 
	GPIOx->BRR = GPIO_Pin;
}

/*******************************************************************************
�� �� ����	void GPIO_ToggleBits(GPIO_Port GPIO_x, BF_INT16U pin)
����˵���� 	ȡ���������״̬(reverse)
��	  ���� 	GPIOx   : ��Ӧ��GPIO_A~GPIO_G
			GPIO_Pin: �ܽ�GPIO_Pin_0~GPIO_Pin_15
�� �� ֵ��	��
ע    ��:   ��pin = 0xffff; ʱ���Դ˶˿ڵ��������Ž���ȡ��
*******************************************************************************/
void GPIO_ToggleBits(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
{
    GPIO_TypeDef *GPIOx = (GPIO_TypeDef*)(GPIOA_BASE + (GPIO_x<<10)); 
	GPIOx->ODR ^= GPIO_Pin;
}

/*******************************************************************************
�� �� ����	BF_INT08U GPIO_ReadOutputDataBit(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
����˵���� 	��ȡ��Ӧ�˿ڸ����ŵ����״ֵ̬
��	  ���� 	GPIOx: ��Ӧ��GPIO_(A~G)
			pin	 : �ܽ�pin_(0~15)
�� �� ֵ��	�����޷���8λ����,�������ű�������Ϊ1,����Ϊ0;
ע    ��: 	��
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
�� �� ����	BF_INT08U GPIO_ReadInputDataBit(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
����˵���� 	��ȡ��Ӧ�˿ڸ����ŵ�����״ֵ̬
��	  ���� 	GPIOx: ��Ӧ��GPIO_(A~G)
			pin	 : �ܽ�pin_(0~15)
�� �� ֵ��	�����޷���8λ����,�������ű�������Ϊ1,����Ϊ0;
ע    ��: 	��
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
�� �� ����	void GPIO_PinLockConfig(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
����˵���� 	������ӦGPIO����Ӧ�ܽ�
��	  ���� 	GPIOx: ��Ӧ��GPIO_(A~G)
			pin	 : �ܽ�pin_(0~15)
�� �� ֵ��	��
ע    ��: 	��д�����ڼ�,���ܸ���LCKR[15:0] ��ֵ;��LOCK �� �� Ӧ ��
			��ĳ���˿�λ��,��ִ����һ�θ�λ֮ǰ,���޷��Ըö˿�λ��ֵ�����޸�;
*******************************************************************************/
void GPIO_PinLockConfig(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
{
    volatile BF_INT32U tmp = 0x00010000; //ʹ�ܶ˿�λ��������
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
�� �� ����	void GPIO_PinRemapConfig(BF_INT32U GPIO_Remap, FunctionalState NewState)
����˵���� 	����GPIO����ӳ�书��
��	  ���� 	GPIO_Remap    : GPIO_Remap_SPI1 ~ GPIO_Remap_MISC (������ο�ͷ�ļ�)
			NewState      : ENABLE��ʾʹ��,DISENABLE��ʾ����
�� �� ֵ��	��
ע    ��: 	��
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
