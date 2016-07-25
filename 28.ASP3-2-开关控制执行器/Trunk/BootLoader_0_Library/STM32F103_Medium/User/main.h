/***************************Copyright BestFu 2016-04-21 *************************
文	件：	main.h
说	明：	系统主函数头文件
编	译：	uVision V5.12.0
版	本：	v1.0
编	写：	jay
日	期：	2016-04-21 
修　改：	暂无
*******************************************************************************/

/*********************************自定义宏*************************************/
#define DEVICE_PARTION_TABLE_ADDR	((unsigned int)(0x08000380))		//设备分区表的起始地址
#define BOOTLOADER_0_JUMP_FLAG		((unsigned int)(0xAA788755))		//标记bootloader0的跳转方向

/********************************数据结构定义**********************************/ 
typedef struct		//bootloader_0_flag区信息
{
	unsigned int bootloader_0_flag;			//Bootloader_0程序跳转标记
	unsigned int bootloader_1_Softver;		//bootloader1软件版本
	unsigned int bootloader_1_AppSize;		//bootloader1软甲大小
}Bootloader0Info_t;

typedef struct		//设备分区表
{
	unsigned int boot0_start_addr;			//bootloader0的起始地址
	unsigned int boot0_flag_start_addr;		//bootloader0_flag的起始地址
	unsigned int boot1_start_addr;			//bootloader1的起始地址
	unsigned int boot1_flag_start_addr;		//bootloader1_flag的起始地址
	unsigned int user_app_start_addr;		//用户app的起始地址
	unsigned int user_data_start_addr;		//用户数据区的保留地址
	unsigned int flash_end_addr;			//用户flash的结束地址
}DevPartionTable_t;


#define DevPartTab			((DevPartionTable_t*)(DEVICE_PARTION_TABLE_ADDR))	//设备分区表的地址信息
/**************************Copyright BestFu 2016-04-21*************************/
