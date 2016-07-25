/***************************Copyright BestFu 2014-05-14*************************
文	件：    Sys.c
说	明：    STM32内核相关设备函数
编	译：    Keil uVision4 V4.54.0.0
版	本：    v1.0
编	写：    Unarty
日	期：    2013-08-24
修　改：	暂无
*******************************************************************************/
	 
#include "stm32f10x.h"
#include "BF_type.h"
#include "sys.h"

//禁止全部中断
__asm void DisableIRQ(void)
{
	CPSID I;
	BX   LR;
}
//使能全部中断
__asm void EnableIRQ(void)
{
	CPSIE I; 
	BX   LR;
}
/*******************************************************************************
函 数 名：void MY_NVIC_SetVectorTable(u32 NVIC_VectTab, u32 Offset)
功能说明：设置向量表偏移地址
参	  数：NVIC_VectTab:基址	 Offset:偏移量
返 回 值：无
*******************************************************************************/
void MY_NVIC_SetVectorTable(u32 NVIC_VectTab, u32 Offset)	 
{  	 
	SCB->VTOR = NVIC_VectTab|(Offset & (u32)0x1FFFFF80);//设置NVIC的向量表偏移寄存器
	//用于标识向量表是在CODE区还是在RAM区
}

/*******************************************************************************
函 数 名：void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group)	
功能说明：设置NVIC分组
参	  数：NVIC_Group:NVIC分组 0~4 总共5组 
返 回 值：无
*******************************************************************************/
void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group)	 
{ 
	u32 temp,temp1;	  
	temp1=(~NVIC_Group)&0x07;//取后三位
	temp1<<=8;
	temp=SCB->AIRCR;  //读取先前的设置
	temp&=0X0000F8FF; //清空先前分组
	temp|=0X05FA0000; //写入钥匙
	temp|=temp1;	   
	SCB->AIRCR=temp;  //设置分组	    	  				   
}

/*******************************************************************************
函 数 名：void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group)		
功能说明：设置NVIC 
参	  数：NVIC_PreemptionPriority:抢占优先级
			NVIC_SubPriority       :响应优先级
			NVIC_Channel           :中断编号
			NVIC_Group             :中断分组 0~4
			注意优先级不能超过设定的组的范围!否则会有意想不到的错误
			组划分:
			组0:0位抢占优先级,4位响应优先级
			组1:1位抢占优先级,3位响应优先级
			组2:2位抢占优先级,2位响应优先级
			组3:3位抢占优先级,1位响应优先级
			组4:4位抢占优先级,0位响应优先级
			NVIC_SubPriority和NVIC_PreemptionPriority的原则是,数值越小,越优先
返 回 值：无
*******************************************************************************/
void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group)	 
{ 
	u32 temp;	
	u8 IPRADDR=NVIC_Channel/4;  //每组只能存4个,得到组地址 
	u8 IPROFFSET=NVIC_Channel%4;//在组内的偏移
	IPROFFSET=IPROFFSET*8+4;    //得到偏移的确切位置
	MY_NVIC_PriorityGroupConfig(NVIC_Group);//设置分组
	temp=NVIC_PreemptionPriority<<(4-NVIC_Group);	  
	temp|=NVIC_SubPriority&(0x0f>>NVIC_Group);
	temp&=0xf;//取低四位

	if(NVIC_Channel<32)NVIC->ISER[0]|=1<<NVIC_Channel;//使能中断位(要清除的话,相反操作就OK)
	else NVIC->ISER[1]|=1<<(NVIC_Channel-32);    
	NVIC->IP[IPRADDR]|=temp<<IPROFFSET;//设置响应优先级和抢断优先级   	    	  				   
}

/*******************************************************************************
函 数 名：void Ex_NVIC_Config(u8 GPIOx,u8 BITx,u8 TRIM) 
功能说明：外部中断配置函数
参	  数：只针对GPIOA~G;不包括PVD,RTC和USB唤醒这三个
			参数:GPIOx:0~6,代表GPIOA~G;BITx:需要使能的位;TRIM:触发模式,1,下升沿;2,上降沿;3，任意电平触发
			该函数一次只能配置1个IO口,多个IO口,需多次调用
			该函数会自动开启对应中断,以及屏蔽?  
返 回 值：无
*******************************************************************************/
void Ex_NVIC_Config(u8 GPIOx,u8 BITx,u8 TRIM) 
{
	u8 EXTADDR;
	u8 EXTOFFSET;
	EXTADDR=BITx/4;//得到中断寄存器组的编号
	EXTOFFSET=(BITx%4)*4;
	RCC->APB2ENR    |= 1<<0;//使能SYSCFG APB2 Clock.
	
	AFIO->EXTICR[EXTADDR]&=~(0x000F<<EXTOFFSET);//清除原来设置！！！
	AFIO->EXTICR[EXTADDR]|=GPIOx<<EXTOFFSET;//EXTI.BITx映射到GPIOx.BITx


	EXTI->IMR|=1<<BITx;//  开启line BITx上的中断
	//EXTI->EMR|=1<<BITx;//不屏蔽line BITx上的事件 (如果不屏蔽这句,在硬件上是可以的,但是在软件仿真的时候无法进入中断!)
 	if(TRIM&0x01)EXTI->FTSR|=1<<BITx;//line BITx上事件下降沿触发
	if(TRIM&0x02)EXTI->RTSR|=1<<BITx;//line BITx上事件上升降沿触发
} 

//  
///*******************************************************************
//函 数 名：void MYRCC_DeInit(void)
//功能说明：把所有时钟寄存器复位
//参	  数：无
//返 回 值：无
//注	  意：不能在这里执行所有外设复位!否则至少引起串口不工作.
//*******************************************************************/
//void MYRCC_DeInit(void)
//{										  					   
//	RCC->APB1RSTR = 0x00000000;//复位结束			 
//	RCC->APB2RSTR = 0x00000000; 
//	  
//  	RCC->AHBENR = 0x00000014;  //睡眠模式闪存和SRAM时钟使能.其他关闭.	  
//  	RCC->APB2ENR = 0x00000000; //外设时钟关闭.			   
//  	RCC->APB1ENR = 0x00000000;   
//	RCC->CR |= 0x00000001;     //使能内部高速时钟HSION	 															 
//	RCC->CFGR &= 0xF8FF0000;   //复位SW[1:0],HPRE[3:0],PPRE1[2:0],PPRE2[2:0],ADCPRE[1:0],MCO[2:0]					 
//	RCC->CR &= 0xFEF6FFFF;     //复位HSEON,CSSON,PLLON
//	RCC->CR &= 0xFFFBFFFF;     //复位HSEBYP	   	  
//	RCC->CFGR &= 0xFF80FFFF;   //复位PLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE 
//	RCC->CIR = 0x00000000;     //关闭所有中断
//	//配置向量表				  
//#ifdef  VECT_TAB_RAM
//	MY_NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x00);
//#else   
//	MY_NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x00);
//#endif
//}



///*******************************************************************
//函 数 名：void Sys_Standby(void)
//功能说明：进入待机模式
//参	  数：无
//返 回 值：无
//*******************************************************************/
//void Sys_Standby(void)
//{
//	 
//}
///*******************************************************************
//函 数 名：void BKP_Write(u8 reg,u16 dat)
//功能说明：后备寄存器写入操作
//参	  数：reg:寄存器编号
//		  reg:要写入的数值 
//返 回 值：无
//*******************************************************************/	  
////void BKP_Write(u8 reg,u16 dat)
////{
////  RCC->APB1ENR|=1<<28;     //使能电源时钟	    
////	RCC->APB1ENR|=1<<27;     //使能备份时钟	    
////	PWR->CR|=1<<8;           //取消备份区写保护 
////	switch(reg)
////	{
////		case 1:
////			BKP->DR1=dat;
////			break;
////		case 2:
////			BKP->DR2=dat;
////			break;
////		case 3:
////			BKP->DR3=dat;
////			break; 
////		case 4:
////			BKP->DR4=dat;
////			break;
////		case 5:
////			BKP->DR5=dat;
////			break;
////		case 6:
////			BKP->DR6=dat;
////			break;
////		case 7:
////			BKP->DR7=dat;
////			break;
////		case 8:
////			BKP->DR8=dat;
////			break;
////		case 9:
////			BKP->DR9=dat;
////			break;
////		case 10:
////			BKP->DR10=dat;
////			break;
////	} 
////}	

///*******************************************************************
//函 数 名：void Sys_Soft_Reset(void)
//功能说明：系统软复位
//参	  数：无
//返 回 值：无
//*******************************************************************/	     
//void Sys_Soft_Reset(void)
//{   
//	SCB->AIRCR =0X05FA0000|(u32)0x04;	  
//} 

///*******************************************************************
//函 数 名：void JTAG_Set(u8 mode)
//功能说明：JTAG模式设置,用于设置JTAG的模式
//参	  数：mode:jtag,swd模式设置;00,全使能;01,使能SWD;10,全关闭;
//返 回 值：无
//*******************************************************************/	  
//void JTAG_Set(u8 mode)
//{
//	u32 temp;
//	temp=mode;
//	temp<<=25;
//	RCC->APB2ENR|=1<<0;     //开启辅助时钟	   
////	AFIO->MAPR&=0XF8FFFFFF; //清除MAPR的[26:24]
////	AFIO->MAPR|=temp;       //设置jtag模式
//} 

///*******************************************************************
//函 数 名：void Stm32_Clock_Init(u8 PLL)
//功能说明：系统时钟初始化函数
//参	  数：pll:选择的倍频数，从2开始，最大值为16	
//返 回 值：无
//*******************************************************************/
//void Stm32_Clock_Init(u8 PLL)
//{

////	unsigned char temp=0;   
////	MYRCC_DeInit();		  //复位并配置向量表
////	RCC->CR|=0x00010000;  //外部高速时钟使能HSEON
////	while(!(RCC->CR>>17));//等待外部时钟就绪
////	RCC->CFGR=0X00000400; //APB1=DIV2;APB2=DIV1;AHB=DIV1;
////	PLL-=2;//抵消2个单位
////	RCC->CFGR|=PLL<<18;   //设置PLL值 2~16
////	RCC->CFGR|=1<<16;	  //PLLSRC ON 
////	FLASH->ACR|=0x32;	  //FLASH 2个延时周期
////
////	RCC->CR|=0x01000000;  //PLLON
////	while(!(RCC->CR>>25));//等待PLL锁定
////	RCC->CFGR|=0x00000002;//PLL作为系统时钟	 
////	while(temp!=0x02)     //等待PLL作为系统时钟设置成功
////	{   
////		temp=RCC->CFGR>>2;
////		temp&=0x03;
////	}   

///*Enable HSE ----------------------------*/
//RCC->CR |= ((uint32_t)RCC_CR_HSEON);  //外部高速时钟使能HSEON
///* Wait till HSE is ready */
//while((RCC->CR & RCC_CR_HSERDY) == 0); //等待外部时钟就绪
///* Flash configuration -------------------*/
///* Flash prefetch and 64-bit access ON, Flash 1 wait
//state */
//FLASH->ACR |= FLASH_ACR_ACC64;
//FLASH->ACR |= FLASH_ACR_PRFTEN;
//FLASH->ACR |= FLASH_ACR_LATENCY;
///* Power enable */
//RCC->APB1ENR |= RCC_APB1ENR_PWREN;
///* Select the Voltage Range 1 (1.8 V) */
//PWR->CR = PWR_CR_VOS_0;
///* Wait Until the Voltage Regulator is ready */
//while((PWR->CSR & PWR_CSR_VOSF) != RESET)
//{
//}
///* AHB and APB prescaler configuration --*/
///* HCLK = SYSCLK */
//RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
///* PCLK2 = HCLK / 1*/
//RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;
///* PCLK1 = HCLK / 1*/
//RCC->CFGR |= RCC_CFGR_PPRE1_DIV1;
///* PLL configuration ---------------------*/
///* PLLCLK = (HSE * PLL_MUL) / PLL_DIV
//= (8 MHz * 12) / 3
//= 32MHz */
//RCC->CFGR = RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMUL12 | RCC_CFGR_PLLDIV3;	 //8*3/3=8 8*6/3=16 8*12/3=32
///* Enable the main PLL */
//RCC->CR |= RCC_CR_PLLON;
///* Wait till the main PLL is ready */
//while((RCC->CR & RCC_CR_PLLRDY) == 0);

///* -- Main PLL used as system clock source --*/

//RCC->CFGR |= RCC_CFGR_SW_PLL;

///* Wait till the main PLL is used as system
//clock source */
//while ((RCC->CFGR & RCC_CFGR_SWS ) !=RCC_CFGR_SWS_PLL);

// 
//}


/**< 恢复中断 */
__asm void CPU_SR_Restore(u32 cpu_sr)
{
    MSR     PRIMASK, R0
    BX      LR
}

/**< 保存中断状态，关中断 */
__asm u32 CPU_SR_Save(void)
{
    MRS     R0, PRIMASK
    CPSID   I
    BX      LR
}

/**************************Copyright BestFu 2014-05-14*************************/ 

