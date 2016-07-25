/***************************Copyright BestFu 2016-04-13*************************
文	件：	lib_config.h
说	明：	整个库文件的配置文件
编	译：	uVision V5.12.0
版	本：	v1.0
编	写：	Jay
日	期：	2016-04-13 
修　改：	None
*******************************************************************************/
#ifndef LIB_CONFIG_H
#define LIB_CONFIG_H

/*Max Thread number is MAX_THREAD_NUM*/
#define MAX_THREAD_NUM      			(20)		//最多线程总数 

/* 配置flash加密功能 */
#define FLASH_ENCRYPTION_EN				(0u)		//使能flash加密功能

/* 配置设备地址重设功能 */
#define DEVICE_ADDR_RESET_FEATURE_EN	(1u)		//使能地址重设功能

/* 配置消息的来源 */
#define RF_433M_INTERFACE_EN			(1u)		//开启433M通讯
#define USART_INTERFACE_EN				(1u)		//开启串口通讯
#define USB_INTERFACE_EN				(0u)		//开启USB通讯
#define INTERNET_INTERFACE_EN			(0u)		//开启网络通讯

/* 通讯的缓冲队列大小分配 */
#if (RF_433M_INTERFACE_EN > 0u)
#define RF_433M_INTERFACE_BUF_SIZE		(0x400)		//1k
#endif
#if (USART_INTERFACE_EN > 0u )
#define USART_INTERFACE_BUF_SIZE		(0x400)		//1k
#endif
#if (USB_INTERFACE_EN > 0u)
#define USB_INTERFACE_BUF_SIZE			(0x400)		//1k
#endif
#if (INTERNET_INTERFACE_EN > 0u)
#define INTERNET_INTERFACE_BUF_SIZE		(0x400)		//1k
#endif


#if (!(defined(RF_433M_INTERFACE_EN)||(defined(USART_INTERFACE_EN))||\
	(defined(USB_INTERFACE_EN))||(defined(INTERNET_INTERFACE_EN))||\
	((0u == RF_433M_INTERFACE_EN)&&(0u == USART_INTERFACE_EN)&&\
	(0u == USB_INTERFACE_EN)&&(0u == INTERNET_INTERFACE_EN))))
#error "Please define Interface mode!"
#endif

#if (MAX_THREAD_NUM >= 25)
#error  "Max MAX_THREAD_NUM is 25"
#endif

#endif
