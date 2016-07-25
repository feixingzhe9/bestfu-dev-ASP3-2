/***************************Copyright xxxxx 2015-12-29*************************
**  文	  件：sys.c
**  说	  明：系统公用处理函数
**  编	  译：uVision V5.12.0.0
**  版	  本：V1.0
**  编	  写：Jay
**  创建日期：2015/12/29
**  修改日期：无
**  说    明：初版
*******************************************************************************/ 
#include "stm32f10x.h"
#include "lib_type.h"
#include "lib_config.h"
#include "sys.h" 

/*******************************************************************************
函 数 名:  	void MY_NVIC_SetVectorTable(BF_INT32U NVIC_VectTab, BF_INT32U Offset)
功能说明:  	设置向量表偏移地址
参    数:  	NVIC_VectTab:基址
			Offset:偏移量	
返 回 值:  	无
说    明:	无
*******************************************************************************/
void MY_NVIC_SetVectorTable(BF_INT32U NVIC_VectTab, BF_INT32U Offset)	 
{ 	   	 
	SCB->VTOR = NVIC_VectTab|(Offset & (BF_INT32U)0x1FFFFF80);	//设置NVIC的向量表偏移寄存器
}

/*******************************************************************************
函 数 名:  	void MY_NVIC_PriorityGroupConfig(BF_INT08U NVIC_Group)	
功能说明:  	设置NVIC分组
参    数:  	NVIC_Group:NVIC分组 0~4 总共5组,取值NVIC_PriorityGroup_0~NVIC_PriorityGroup_4
返 回 值:  	无
说    明:	无
*******************************************************************************/
void MY_NVIC_PriorityGroupConfig(BF_INT08U NVIC_Group)	 
{ 
	BF_INT32U temp,temp1;	  
	temp1   = (~NVIC_Group) & 0x07;	//取后三位
	temp1 <<= 8;
	temp    = SCB->AIRCR;  			//读取先前的设置
	temp   &= 0X0000F8FF; 			//清空先前分组
	temp   |= 0X05FA0000; 			//写入钥匙
	temp   |= temp1;	   
	SCB->AIRCR=temp;  				//设置分组	    	  				   
}

/*******************************************************************************
函 数 名:  	void MY_NVIC_Init(BF_INT08U NVIC_PreemptionPriority,BF_INT08U NVIC_SubPriority,BF_INT08U NVIC_Channel,BF_INT08U NVIC_Group)	
功能说明:  	设置NVIC
参    数:  	NVIC_PreemptionPriority:抢占优先级
			NVIC_SubPriority       :响应优先级
			NVIC_Channel           :中断编号
			NVIC_Group             :中断分组 NVIC_PriorityGroup_0~NVIC_PriorityGroup_4
返 回 值:  	无
说    明:	组划分:
			组0:0位抢占优先级,4位响应优先级
			组1:1位抢占优先级,3位响应优先级
			组2:2位抢占优先级,2位响应优先级
			组3:3位抢占优先级,1位响应优先级
			组4:4位抢占优先级,0位响应优先级
			NVIC_SubPriority和NVIC_PreemptionPriority的原则是,数值越小,越优先	
*******************************************************************************/
void MY_NVIC_Init(BF_INT08U NVIC_PreemptionPriority,BF_INT08U NVIC_SubPriority,BF_INT08U NVIC_IRQChannel,BF_INT08U NVIC_Group)	 
{ 
	BF_INT32U tmppriority = 0x00u;
	
	/* Compute the Corresponding IRQ Priority --------------------------------*/ 
	MY_NVIC_PriorityGroupConfig(NVIC_Group);					//设置分组
	tmppriority	= NVIC_PreemptionPriority << (0x4 - NVIC_Group);//抢占优先级设置
	tmppriority|= NVIC_SubPriority & (0x0f >> NVIC_Group);		//响应优先级设置
	tmppriority <<= 0x04;
	NVIC->IP[NVIC_IRQChannel] = tmppriority;
	/* Enable the Selected IRQ Channels --------------------------------------*/
	NVIC->ISER[NVIC_IRQChannel >> 0x05] = 						
		(BF_INT32U)0x01 << (NVIC_IRQChannel & (BF_INT08U)0x1F); //使能中断位(要清除的话,相反操作就OK)
}

/*******************************************************************************
函 数 名:  	void EXTI_NVIC_Config(BF_INT08U GPIOx,BF_INT08U BITx,BF_INT08U TRIM) 
功能说明:  	外部中断配置函数
参    数:  	GPIOx:取值EXTI_PortSourceGPIOA ~ EXTI_PortSourceGPIOI
			BITx:取值EXTI_PinSource0 ~ EXTI_PinSource15
			TRIM:取值(EXTI_Trigger_Falling,EXTI_Trigger_Rising,EXTI_Trigger_Rising_Falling)
返 回 值:  	无
说    明:	该函数一次只能配置1个IO口,多个IO口,需多次调用
			该函数会自动开启对应中断,以及屏蔽线  
*******************************************************************************/
void EXTI_NVIC_Config(BF_INT08U GPIOx,BF_INT08U BITx,BF_INT08U TRIM) 
{
	BF_INT08U EXTADDR;
	BF_INT08U EXTOFFSET;
	EXTADDR=BITx/4;								//得到中断寄存器组的编号
	EXTOFFSET=(BITx%4)*4; 
	RCC->APB2ENR|=0x01;							//使能io复用时钟			 
	AFIO->EXTICR[EXTADDR]&=~(0x000F<<EXTOFFSET);//清除原来设置！！！
	AFIO->EXTICR[EXTADDR]|=GPIOx<<EXTOFFSET;	//EXTI.BITx映射到GPIOx.BITx 
	EXTI->IMR|=1<<BITx;							//开启line BITx上的中断
	//EXTI->EMR|=1<<BITx;//不屏蔽line BITx上的事件 (如果不屏蔽这句,在硬件上是可以的,但是在软件仿真的时候无法进入中断!)
 	if(TRIM&0x01)EXTI->FTSR|=1<<BITx;			//line BITx上事件下降沿触发
	if(TRIM&0x02)EXTI->RTSR|=1<<BITx;			//line BITx上事件上升降沿触发
} 	

/*******************************************************************
函 数 名：	Jump_To_Code
功能说明：	跳转到地址对应的程序区运行
参	  数：	addr:	对代码存储区首地址
返 回 值：	无
*******************************************************************/
void JumpToCode(BF_INT32U ApplicationAddress)
{
	volatile BF_INT32U* p = (volatile BF_INT32U*)ApplicationAddress;
	if (((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)
	{
		Set_MSP(*p);		//取出初始的MSP的值
		((void (*)(void))(*(p + 1)))();
	}
}
/**********************************End of File*********************************/
