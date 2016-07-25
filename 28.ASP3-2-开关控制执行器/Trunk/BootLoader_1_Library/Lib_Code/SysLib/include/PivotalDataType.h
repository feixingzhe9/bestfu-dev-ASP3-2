/***************************Copyright BestFu 2014-05-14*************************
文	件：    PivotalDataType.h
说	明:		库关键数据类型头文件
编	译：    uVision V5.12.0
版	本：    v1.0
编	写：    jay
日	期：    2016-04-14
修  改:     暂无
*******************************************************************************/
#ifndef __PIVOTAL_DATA_TYPE_H   
#define __PIVOTAL_DATA_TYPE_H   

/*********************************自定义宏*************************************/
#define CRC_SIZE					(0x02)			//CRC数据占用空间，单位：byte
#define MAX_DATA_FRAME_LEN			(0xFA)			//一串指令的最长长度
#define HEAD_SIZE					(6)				//头数据长度
#define TAIL_SIZE					(6)				//尾数据长度
#define ARGE_SEAT					(1)				//参数据位置

#define EVENT						(0x00)
#define COMMUNICATION_VERSION		(0x01)			//通信版本
#define DECRYPT_COMMUNICATION   	(0x02)      	//加密通信版本
#define MSG_UPLOAD_ID				(0x88888888)	//消息上报地址

//定义数据的通讯来源
#define RF_433M_INTERFACE			((BF_INT08U)(1))//定义433通讯的源标记	
#define USART_INTERFACE				((BF_INT08U)(2))//定义串口通讯的源标记
#define USB_INTERFACE				((BF_INT08U)(3))//定义USB通讯的源标记
#define INTERNET_INTERFACE			((BF_INT08U)(4))//定义因特网通讯的源标记

/*********************************数据类型定义*********************************/
typedef enum
{
	COMPLETE 				= 1,	//操作正常完成
	
	OWN_RETURN_PARA			= 2,	//拥有返回参数	
	NO_ACK					= 3,	//不做应答
	USER_ID_ERR				= 4,	//用户号错误
	ADDS_TYPE_ERR			= 5,	//目标地址类型错误	
	ID_ERR					= 6,	//目标地址错误
	
	AREA_ERR				= 7,	//区域错误
	MSG_REPEAT				= 8,	//消息重复
	UNIT_DISABLE			= 9,	//单元禁能
	NO_WRITE				= 10,	//属性没有写权限
	NO_READ					= 11,	//属性没读权限
	
	MSG_TYPE_ERR			= 12,	//消息类型错误
	NO_CMD					= 13,	//单元属性不存在
	PARA_MEANING_ERR		= 14,	//参数意义错误
	PARA_LEN_ERR			= 15,	//参数长度错误
	EEPROM_ERR				= 16,	//EEPROM操作错误
	
	EEPROM_RAND_ERR			= 17,	//EEPROM操作范围越界
	CMD_EXE_ERR				= 18,	//指令执行错误
	
	LINKFULL				= 19,   //联动满，无法再写
	LINKNUM_ERR				= 20,	//联动号获取失败
	LINKMAP_ERR				= 21,   //联动操作类型码和联动操作类型匹配错误
	LINK_MODIFY_ERR			= 22,	//联动修改错误
	LINK_WRITE_ERR			= 23,	//联动写入失败
	
	CMD_EXE_OVERTIME		= 24,	//指令执行超时	
	DATA_SAVE_OVER			= 25,	//数据存储满
	DATA_NO_FIND			= 26,	//数据没有找到	
	VOICE_POWER_BAT			= 27,	//语音处于电池供电状态	
	
	DLY_BUFFER_FULL			= 28,	//延时缓冲队列已满		
	TIME_PARA_ERR			= 29,	//时间参数错误
	
	BOOT_UPGRADE_AIM_ERR	= 30,	//引导程序不能升级引导程序
	BOOT_LOSE_ERR			= 31,	//bootloader引导程序丢失	
	APP_SIZE_BOUND_ERR		= 32,	//升级APP大小越界
	PACKET_SIZE_ALIGN_ERR	= 33,	//包大小字节对齐错误	
	UPGRADE_STATE_ERR		= 34,	//升级状态错误
	
	FLASH_SPACE_FULL_ERR	= 35,	//flash空间已满错误
	
}MsgResult_t;						//消息操作结果枚举量

//通讯协议相关结构体定义
typedef enum
{
	WRITEWITHACK 	= 0x02,		//需应答写操作消息类型码
    ACKWRITE		= 0x03,     //写操作应答消息类型码
    WRITENACK       = 0x04,    	//无需应答写操作消息类型码

    READWITHACK     = 0x05,    	//读操作消息类型码
    READACK         = 0x06,    	//读操作应答消息类型码

    WRITESUC        = 0x07,    	// 写操作成功时才回应消息类型码
    WRITEACKSUC     = 0x08,    	// 写操作成功时应答消息类型码
    WRITEFAIL       = 0x09,    	// 写操作失败时才回应消息类型码
    WRITEACKFAIL    = 0x0A,   	// 写操作失败时应答消息类型码

    READSUC         = 0x0B,    	// 读操作成功时才回应消息类型码
    READACKSUC      = 0x0C,    	// 读操作成功时应答消息类型码
    READFAIL        = 0x0D,    	// 读操作失败时才回应消息类型码
    READACKFAIL     = 0x0E,    	// 读操作失败时应答消息类型码
}MsgRcve_t;

typedef enum
{
	SINGLE_ACT 			         = 0X01,		//单节点操作
	GROUP_ACT 			         = 0X02,		//组广播
	SCENE_ACT			         = 0X03,		//情景广播
	DEVIVCE_TYPE_ACT 	         = 0x04,		//设备类型广播 
	DEV_ALL_ACT      	         = 0x05,    	//所有广播
	UNIT_USER_TYPE_ACT	         = 0x06,		//单元应用类型广播
	UNIT_COMMON_TYPE_ACT         = 0x07,		//单元应用大类广播
	SPEC_AREA_DEV_TYPE_ACT  	 = 0x14,		//特定区域设备类型广播				/**>  Jay Add 2015.11.17*/
	SPEC_AREA_UNIT_USER_TYPE_ACT = 0x16			//特定区域单元应用类型广播			/**>  Jay Add 2015.11.17*/
}ObjectType_t;

#pragma pack(1)
typedef union
{
    BF_INT32U 	id;
    BF_INT08U	area[4];
}Object_t;      //目标描述共同体

typedef struct 
{
	BF_INT32U	userID;     	// 用户号
	BF_INT08U	objectType;    	// 目标地址类型
	Object_t 	object;			// 目标地址
	BF_INT16U	actNum;  		// 广播类型号
	BF_INT32U 	sourceID;   	// 源地址
	BF_INT08U	serialNum;      // 流水号
}CMDAim_t;	//通信目类型描述

typedef struct 
{
	BF_INT08U	msgType;		// 消息类型码
	BF_INT08U	unit;           // 单元ID
	BF_INT08U	cmd; 			// 命令码
	BF_INT08U	len;		  	// 参数长度
}CMDPara_t;						//通信参数描述

typedef struct
{
	BF_INT08U	version;   		// 版本号
	CMDAim_t 	aim;			//目标类型描述
	CMDPara_t	para;		
}Communication_t;
#pragma pack()

//单元数据处理相关数据类型定义
typedef enum
{
	LEVEL_0,					//等级最低
	LEVEL_1,					//
}Level_t;
#pragma pack(1)
typedef struct
{
	const	BF_INT08U msgsrc;	//消息来源
	const 	BF_INT08U unit;
	const 	BF_INT08U cmd;
			BF_INT08U len;
	const 	BF_INT08U data[200];
}UnitPara_t;
#pragma pack()

typedef struct
{
	BF_INT32U cmd;                                   		//指令号
	Level_t level;											//属性重要级别
	MsgResult_t (*pSet_Attr)(UnitPara_t *pData);    		//set函数入口
	MsgResult_t (*pGet_Attr)(UnitPara_t *pData,\
						BF_INT08U *rlen, BF_INT08U *rpara);	//get函数入口
}AttrExe_st;

typedef struct
{
	void (*Init)(BF_INT08U unitID);			// 单元初始化，包括自身全局参数和驱动初始化
	void (*DefEE)(BF_INT08U unitID);		// EEPROM参数设定默认值
	void (*Instruct)(void);					//单元验证函数
	const AttrExe_st *attrExe;				//属性列表指针
	BF_INT08U (*ExeAttrNum)(void);			//获取单元属性的数量
}Unit_st;

/*********************************数据声明*************************************/
extern const BF_INT08U HeadCMD[HEAD_SIZE];
extern const BF_INT08U TailCMD[TAIL_SIZE];

#endif 	//ComStructure.h
/**************************Copyright BestFu 2014-05-14*************************/
