/***************************Copyright BestFu 2014-05-14*************************
文	件：	i2c.h
说	明：	I2C模式通信驱动
编	译：	uVision V5.18.0
版	本：	v1.0
编	写：	Unarty
日	期：	2014.05.04  
修　改：	2016.04.23 Modify by jay
*******************************************************************************/
#ifndef __I2C_H
#define __I2C_H

/*********************************自定义宏*************************************/
#define GPIO_OUT0(GPIOx, pin)  	GPIO_ResetBits(GPIOx, pin)	//GPIO口置0
#define GPIO_OUT1(GPIOx, pin)  	GPIO_SetBits(GPIOx, pin)	//GPIO口置1

#if (SCL_PORT == SCL_PORT)
#define	SCL_SDA_INIT			GPIO_Config(SDA_PORT,SDA_PIN | SCL_PIN,OUT_PP_50M);
#else
#define	SCL_SDA_INIT			GPIO_Config(SDA_PORT,SDA_PIN,OUT_PP_50M);\
								GPIO_Config(SCL_PORT,SCL_PIN,OUT_PP_50M)
#endif

#define SDA(f)        			GPIO_OUT##f(SCL_PORT, SDA_PIN)     
#define SCL(f)                  GPIO_OUT##f(SCL_PORT, SCL_PIN)


#define SDA_OUT                 GPIO_Config(SDA_PORT, SDA_PIN, OUT_PP_50M);\
								GPIO_SetBits(SDA_PORT, SDA_PIN)
								
#define SDA_IN                  GPIO_Config(SDA_PORT, SDA_PIN, IN_UP)

#define SDA_READ                GPIO_ReadInputDataBit(SDA_PORT, SDA_PIN)

#ifdef	WP_PORT		//如果定义写保护端口
	#define WP_INIT				GPIO_Config(WP_PORT, WP_PIN, OUT_PP_50M);\
								GPIO_SetBits(WP_PORT, WP_PIN)
									
	#define WP_UN				GPIO_ResetBits(WP_PORT, WP_PIN)
	#define WP_EN				GPIO_SetBits(WP_PORT, WP_PIN)
#else
	#define WP_INIT	
	#define WP_UN
	#define WP_EN
#endif

/*********************************函数声明*************************************/
void I2C_Init(void);
BF_INT08U IIC_Read(BF_INT08U sla, BF_INT16U suba, BF_INT08U *data, BF_INT08U len);
BF_INT08U IIC_Write(BF_INT08U sla, BF_INT16U suba, BF_INT08U *data, BF_INT08U len);
BF_INT08U IIC_WriteSame(BF_INT08U sla, BF_INT16U suba, BF_INT08U data, BF_INT08U len);
BF_INT08U IIC_GetState(BF_INT08U sla, BF_INT32U overTime);

#endif
/**************************Copyright BestFu 2014-05-14*************************/
