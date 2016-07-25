/***************************Copyright BestFu 2014-05-14*************************
文	件：	i2c.c
说	明：	I2C模式通信驱动
编	译：	uVision V5.18.0
版	本：	v1.0
编	写：	Unarty
日	期：	2014.05.04  
修　改：	2016.04.23 Modify by jay
*******************************************************************************/
#include "lib_type.h"
#include "lib_config.h"
#include "gpio.h"
#include "SysHard.h"
#include "i2c.h"

#define WaitTime	(2)		//等待时间 
#define NOP			(5)		//空闲时间

//Private function declaration
static void Delay_nNOP(BF_INT16U n);
static void I2C_Start(void);
static void  I2C_Stop(void);
static void  I2C_ACK(BF_INT08U ack);
static BF_BOOLEAN I2C_WaitAck(void);
static BF_BOOLEAN I2C_SendB_Ack(BF_INT08U data);
static BF_INT08U I2C_RecvB_Ack(BF_INT08U ack);

/*******************************************************************************
函 数 名：	Delay_nNOP
功能说明： 	延时n个空操作
参	  数： 	n-空操作的个数
返 回 值：	void
*******************************************************************************/
static void Delay_nNOP(BF_INT16U n)
{
	BF_INT16U i = 0;

	for ( ; n > 0; n--)
	{
		for(i = NOP; i > 0; i--)
		{}
	}
}

/*******************************************************************************
函 数 名：	I2C_Init
功能说明： 	I2C的初始化
参	  数： 	void
返 回 值：	void
*******************************************************************************/
void I2C_Init(void)
{
    SCL_SDA_INIT;
    SDA(1);
	SCL(1);
	WP_INIT;
}

/*******************************************************************************
函 数 名：	I2C_Start
功能说明： 	I2C的开始
参	  数： 	void
返 回 值：	void
*******************************************************************************/
static void I2C_Start(void)
{
    SDA(1);         
    SCL(1);         
    Delay_nNOP(WaitTime);
    SDA(0);
    Delay_nNOP(WaitTime);
    SCL(0);
}

/*******************************************************************************
函 数 名：	I2C_Stop
功能说明： 	I2C的停止
参	  数： 	void
返 回 值：	void
*******************************************************************************/
static void I2C_Stop(void)
{
    SDA(0);
    Delay_nNOP(WaitTime);
    SCL(1);
    Delay_nNOP(WaitTime);
    SDA(1);
    Delay_nNOP(WaitTime);
}

/*******************************************************************************
函 数 名：	I2C_ACK
功能说明： 	I2C主动回应
参	  数： 	ack-回应标志，0表示继续读取，1表示读取结束
返 回 值：	void
*******************************************************************************/
static void I2C_ACK(BF_INT08U ack)
{
    SCL(0);
    Delay_nNOP(WaitTime);
    if (ack) 
    {
        SDA(1);
    }
    else 
    {
        SDA(0);
    }
    Delay_nNOP(WaitTime);
    SCL(1);
    Delay_nNOP(WaitTime);
    SCL(0);
}

/*******************************************************************************
函 数 名：	I2C_WaitAck
功能说明： 	I2C等待从器件回应
参	  数： 	void
返 回 值：	BF_INT08U-回应数据，1表示有回应，0表示超时或无回应
*******************************************************************************/
static BF_BOOLEAN I2C_WaitAck(void)
{
	BF_INT16U OutTime=0;
	
	SDA_IN;       //SDA设置为输入  
	
    Delay_nNOP(WaitTime);	   
	SCL(1);
    Delay_nNOP(WaitTime); 
	while (SDA_READ)
	{
		if ((++OutTime) > 500)
		{
			SDA_OUT;
			return FALSE;
		}
	}
	SCL(0);
    Delay_nNOP(WaitTime); 
	SDA_OUT;
	Delay_nNOP(WaitTime);
	
	return TRUE;
}

/*******************************************************************************
函 数 名：	I2C_SendB_Ack
功能说明： 	I2C发送一个字节数据并且等待回应
参	  数： 	data-将要发送的字节
返 回 值：	BF_INT08U-回应数据，1表示有回应，0表示超时或无回应
*******************************************************************************/
static BF_BOOLEAN I2C_SendB_Ack(BF_INT08U data)
{
	BF_INT08U i;
	
	SCL(0);
	Delay_nNOP(WaitTime);
	for (i = 0; i < 8; i++)
	{
		if(data&0x80)
		  SDA(1);
		else
		  SDA(0);
		data <<= 1;
		Delay_nNOP(WaitTime);
		SCL(1);
		Delay_nNOP(WaitTime);
		SCL(0);
		Delay_nNOP(WaitTime);
	}

	return I2C_WaitAck();
}

/*******************************************************************************
函 数 名：	I2C_RecvB_Ack
功能说明： 	I2C接收一个字节数据并且回应ack
参	  数： 	ack-回应信息，0表示继续读取，1-表示停止读取
返 回 值：	BF_INT08U-收到的数据
*******************************************************************************/
static BF_INT08U I2C_RecvB_Ack(BF_INT08U ack)
{
	BF_INT08U i,rcvdata=0;
	
	SDA_IN; 	  //SDA设置为输入  
	for (i = 0; i < 8; i++)
	{
		SCL(0);
		Delay_nNOP(WaitTime);
		SCL(1);
		Delay_nNOP(WaitTime);
		rcvdata <<= 1;

		if(SDA_READ)
		{
			rcvdata++;
		}
		Delay_nNOP(WaitTime);
	}
	SCL(0);
	Delay_nNOP(WaitTime);
	SDA_OUT;
	Delay_nNOP(WaitTime);
	I2C_ACK(ack);
	
	return (rcvdata);
}

/*******************************************************************************
函 数 名：  IIC_GetState
功能说明：  IIC通信设备状态
参	  数：  add:	设备地址
			overtime:等待超时时间
  返回值:  TRUE(非忙)/FALSE(忙)
*******************************************************************************/
BF_BOOLEAN IIC_GetState(BF_INT08U sla, BF_INT32U overTime)
{
	do 
	{	
		I2C_Start();
	}while((!I2C_SendB_Ack(sla)) && --overTime);
	
	I2C_Stop();
	
	return (overTime ?  TRUE : FALSE);
}

/*******************************************************************************
函 数 名：	IIC_Read
功能说明： 	IIC数据读取
参	  数： 	sla-从器件地址
            suba-数据首地址
            data-读取到的数据存放于该指针
            len-长度，最多不能超过一页大小
返 回 值：	1表示成功，0表示失败
*******************************************************************************/
BF_BOOLEAN IIC_Read(BF_INT08U sla, BF_INT16U suba, BF_INT08U *data, BF_INT08U len)
{
	I2C_Start();
	if (I2C_SendB_Ack(sla))
	{	
		if (I2C_SendB_Ack((BF_INT08U)(suba>>8)))
		{
			if (I2C_SendB_Ack((BF_INT08U)(suba)))
			{
				I2C_Start();
				if (I2C_SendB_Ack(sla | 0x01))
				{
					for (; len > 1; len--)
					{
						*data++ = I2C_RecvB_Ack(0);
					}
					len--;
					*data = I2C_RecvB_Ack(1);
				}
			}
		}
	}
	I2C_Stop();
	
	return (len ? FALSE : TRUE);
}

/*******************************************************************************
函 数 名：	I2C_Write
功能说明： 	把一串数据写入到EEPROM中的指定位置
参	  数： 	sla-从器件地址
            suba-数据首地址
            data-要写入的数据存放于该指针
            len-长度，最多不能超过一页大小
返 回 值：	1表示成功，0表示失败
*******************************************************************************/
BF_BOOLEAN IIC_Write(BF_INT08U sla, BF_INT16U suba, BF_INT08U *data, BF_INT08U len)
{
	WP_UN;
	I2C_Start();
	if(I2C_SendB_Ack(sla))
	{
		if (I2C_SendB_Ack((BF_INT08U)(suba>>8)))
		{
			if (I2C_SendB_Ack((BF_INT08U)(suba)))
			{
				for (; len > 0; len--)
				{
					if (!I2C_SendB_Ack(*data++))
					{
						break;
					}
				}
			}
		}
    }
	I2C_Stop();
	WP_EN;
	
	return (len ? FALSE : TRUE);
}

/*******************************************************************************
函 数 名：	I2C_WriteSame
功能说明： 	把一串相同的数据，写入到EEPROM中的指定位置
参	  数： 	sla-从器件地址
            suba-数据首地址
            data-要写入的数据
            len-长度，最多不能超过一页大小
返 回 值：	1表示成功，0表示失败
*******************************************************************************/
BF_BOOLEAN IIC_WriteSame(BF_INT08U sla, BF_INT16U suba, BF_INT08U data, BF_INT08U len)
{
	WP_UN;
	I2C_Start();
	if (I2C_SendB_Ack(sla))
	{
		if (I2C_SendB_Ack((BF_INT08U)(suba>>8)))
		{
			if (I2C_SendB_Ack((BF_INT08U)(suba)))
			{
				for	(; len > 0; len--)
				{
					if (!I2C_SendB_Ack(data))
					{
						break;
					}
				}
			}
		}
    }
	I2C_Stop();
	WP_EN;
	
	return (len ? FALSE : TRUE);
}
/**************************Copyright BestFu 2014-05-14*************************/
