/***************************Copyright BestFu 2016-04-16*************************
文	件：	DevPartTab.h
说	明：	设备分区表处理函数文件
编	译：	uVision V5.12.0
版	本：	v1.0
编	写：	jay
日	期：	2016-04-16
修  改:     暂无
*******************************************************************************/
#include "lib_type.h"
#include "lib_config.h"
#include "program.h"
#include "DevPartTab.h"

//设备关键信息分区表信息
const DevPivotalSet_t DevPivotalSet[3] =
{
    /*    硬件信息 引导0标志区  重设地址   flash加密  软件信息   433信息*/
    {0x080002F0,0x08000400,0x08000430,0x0800043C,0x08005000,0x08005040},	//64K分区信息
    {0x080002F0,0x08000400,0x08000430,0x0800043C,0x08006000,0x08006040},	//128K分区信息
    {0x080002F0,0x08000800,0x08000830,0x0800083C,0x08006800,0x08006840}		//>=256分区信息
};

static DevPivotalSet_t* pDevPivotalSet = NULL;	//定义获取设备关键信息的指针
/*******************************************************************************
函 数 名： 	DevPivotalSetInit
功能说明：  设备关键信息的初始化
参	  数:   无
返 回 值：  无
*******************************************************************************/
void DevPivotalSetInit(void)
{
    BF_INT16U chip_size = Flash_ChipSize();

    if((64   != chip_size)&&(128 != chip_size)&&\
            (256  != chip_size)&&(384 != chip_size)&&\
            (512  != chip_size)&&(768 != chip_size)&&\
            (1024 != chip_size))
    {
        chip_size = (DevPartTab->flash_end_addr - DevPartTab->boot0_start_addr)/1024;
    }
    switch(chip_size)
    {

    case 64:
    {
        pDevPivotalSet  = ((DevPivotalSet_t*)&DevPivotalSet[0]);
    }
    break;
    case 128:
    {
        pDevPivotalSet  = ((DevPivotalSet_t*)&DevPivotalSet[1]);
    }
    break;
    case 256:
    case 384:
    case 512:
    case 768:
    case 1024:
    {
        pDevPivotalSet  = ((DevPivotalSet_t*)&DevPivotalSet[2]);
    }
    break;
    default:
    {
        pDevPivotalSet  = NULL;
    }
    break;
    }
}

/*******************************************************************************
函 数 名： 	GetDevPivotalSet
功能说明：  获取设备关键信息的指针
参	  数:   无
返 回 值：  获取设备关键信息的指针
*******************************************************************************/
DevPivotalSet_t* GetDevPivotalSet(void)
{
    return ((DevPivotalSet_t*)(pDevPivotalSet));
}
/**************************Copyright BestFu 2016-04-16*************************/
