/***************************Copyright BestFu 2015-08-03*************************
文 件:	KeyUnit.c
说 明:	开关执行器驱动性实现文件
编 辑:	Keil uVision5 V5.12
版 本:	v1.0
编 写:	lsc
日 期:	2015.08.03
修 改:	暂无
*******************************************************************************/
#include "Thread.h"
#include "LedUnit.h"

static LED_OFF_t gled_off_opt;
static LED_OFF_t gled_off_Anti_opt;
static u8 gKeyNum = 0x00;
LED_OPT_t LED_OPT[3];  /**> 定义LED结构体 **/

static void LED_ONE_ON(void);
static void LED_TWO_ON(void);
static void LED_ONE_OFF(void);
static void LED_TWO_OFF(void);
static void LED_ONE_RVRS(void);
static void LED_TWO_RVRS(void);
static void LED_THREE_ON(void);
static void LED_THREE_OFF(void);
static void LED_THREE_RVRS(void);
static void LED_OFF_OPT_Func(void);
static void LED_OFF_Anti_OPT_Func(void);
static void LED_ON_OPT_Func(LED_ON_t led_on_opt);

const LEDPort_t LEDPort[3]={{PORT_GPIO_LED1,PIN_LED1},
					        {PORT_GPIO_LED2,PIN_LED2},
                            {PORT_GPIO_LED3,PIN_LED3}};

const LED_Func_PTR_TAB_t LED_Func_PTR_TAB[3] = {{LED_ONE_ON,LED_ONE_OFF,LED_ONE_RVRS},
										        {LED_TWO_ON,LED_TWO_OFF,LED_TWO_RVRS},
										        {LED_THREE_ON,LED_THREE_OFF,LED_THREE_RVRS}};
/*******************************************************************************
函数名:		void LED_UNIT_Init(void)
功能说明：	LED单元初始化
参	数：	无
返回值：	无
*******************************************************************************/
void LED_UNIT_Init(void)
{
   	u8 i = 0;
/*********************************led初始化*********************************/
	for(i = 0;i < BF_DIM(LED_OPT);i++)
	{
		LED_OPT[i].pLEDPort  = (LEDPort_t *)&LEDPort[i];
		LED_OPT[i].LED_ON    = LED_Func_PTR_TAB[i].LED_ON;
		LED_OPT[i].LED_OFF   = LED_Func_PTR_TAB[i].LED_OFF;
		LED_OPT[i].LED_REV   = LED_Func_PTR_TAB[i].LED_REV;
		LED_Init(LED_OPT[i].pLEDPort);
	}
}

/*******************************************************************************
函数名:		static void LED_ONE_ON(void)
功能说明：	LED1开
参	数：	无
返回值：	无
*******************************************************************************/
static void LED_ONE_ON(void)
{
    LED_ON(LED_OPT[0].pLEDPort);
}

/*******************************************************************************
函数名:		static void LED_ONE_OFF(void)
功能说明：	LED1关
参	数：	无
返回值：	无
*******************************************************************************/
static void LED_ONE_OFF(void)
{
    LED_OFF(LED_OPT[0].pLEDPort);
}

/*******************************************************************************
函数名:		static void LED_TWO_OFF(void)
功能说明：	LED一动作取反
参	数：	无
返回值：	无
*******************************************************************************/
static void LED_ONE_RVRS(void)
{
    LED_REV(LED_OPT[0].pLEDPort);
}

/*******************************************************************************
函数名:		static void LED_TWO_ON(void)
功能说明：	LED2开
参	数：	无
返回值：	无
*******************************************************************************/
static void LED_TWO_ON(void)
{
    LED_ON(LED_OPT[1].pLEDPort);
}

/*******************************************************************************
函数名:		static void LED_TWO_OFF(void)
功能说明：	LED2关
参	数：	无
返回值：	无
*******************************************************************************/
static void LED_TWO_OFF(void)
{
    LED_OFF(LED_OPT[1].pLEDPort);
}

/*******************************************************************************
函数名:		void LED_TWO_OFF(void)
功能说明：	LED2动作取反
参	数：	无
返回值：	无
*******************************************************************************/
static void LED_TWO_RVRS(void)
{
    LED_REV(LED_OPT[1].pLEDPort);
}


/*******************************************************************************
函数名:		static void LED_THREE_ON(void)
功能说明：	LED3开
参	数：	无
返回值：	无
*******************************************************************************/
static void LED_THREE_ON(void)
{
    LED_ON(LED_OPT[2].pLEDPort);
}

/*******************************************************************************
函数名:		static void LED_THREE_OFF(void)
功能说明：	LED3关
参	数：	无
返回值：	无
*******************************************************************************/
static void LED_THREE_OFF(void)
{
    LED_OFF(LED_OPT[2].pLEDPort);
}

/*******************************************************************************
函数名:		static void LED_THREE_RVRS(void)
功能说明：	LED3动作取反
参	数：	无
返回值：	无
*******************************************************************************/
static void LED_THREE_RVRS(void)
{
    LED_REV(LED_OPT[2].pLEDPort);
}

/*******************************************************************************
函数名:	 void LED_RVRS_OPT_Func(void)
功能说明：	LED取反
参	数：	无
返回值：	无
*******************************************************************************/
void LED_RVRS_OPT_Func(void)
{
	switch(gKeyNum)
	{
		case 1:
		{
			LED_OPT[0].LED_REV();
		}break;
		case 2:
		{
			LED_OPT[1].LED_REV();
		}break;
		case 3:
		{
			LED_OPT[2].LED_REV();
		}break;
		default:break;
	}
}

/*******************************************************************************
函数名:	 static void LED_ON_OPT_Func(LED_ON_t led_opt)
功能说明：	LED1开
参	数：	无
返回值：	无
*******************************************************************************/
static void LED_ON_OPT_Func(LED_ON_t led_on_opt)
{
	switch(led_on_opt)
	{
		case LED_1_ON:
		{
			LED_ON(LED_OPT[0].pLEDPort);
		}break;
		case LED_2_ON:
		{
			LED_ON(LED_OPT[1].pLEDPort);
		}break;
		case LED_3_ON:
		{
			LED_ON(LED_OPT[2].pLEDPort);
		}break;
		case LED_12_ON:
		{
			LED_ON(LED_OPT[0].pLEDPort);
			LED_ON(LED_OPT[1].pLEDPort);
		}break;
		case LED_13_ON:
		{
			LED_ON(LED_OPT[0].pLEDPort);
			LED_ON(LED_OPT[2].pLEDPort);		
		}break;
		case LED_23_ON:
		{
			LED_ON(LED_OPT[1].pLEDPort);
			LED_ON(LED_OPT[2].pLEDPort);		
		}break;
		case LED_ALL_ON:
		{
			LED_ON(LED_OPT[0].pLEDPort);
			LED_ON(LED_OPT[1].pLEDPort);
			LED_ON(LED_OPT[2].pLEDPort);			
		}break;
		default:break;
	}
}

/*******************************************************************************
函数名:	 static void LED_All_OFF_Func(void)
功能说明：	LED1关
参	数：	无
返回值：	无
*******************************************************************************/
static void LED_OFF_Anti_OPT_Func(void)
{
	switch(gled_off_Anti_opt)
	{
		case LED_1_OFF:
		{
			LED_OFF(LED_OPT[0].pLEDPort);
		}break;
		case LED_2_OFF:
		{
			LED_OFF(LED_OPT[1].pLEDPort);
		}break;
		case LED_3_OFF:
		{
			LED_OFF(LED_OPT[2].pLEDPort);
		}break;
		case LED_12_OFF:
		{
			LED_OFF(LED_OPT[0].pLEDPort);
			LED_OFF(LED_OPT[1].pLEDPort);
		}break;
		case LED_13_OFF:
		{
			LED_OFF(LED_OPT[0].pLEDPort);
			LED_OFF(LED_OPT[2].pLEDPort);		
		}break;
		case LED_23_OFF:
		{
			LED_OFF(LED_OPT[1].pLEDPort);
			LED_OFF(LED_OPT[2].pLEDPort);		
		}break;
		case LED_ALL_OFF:
		{
			LED_OFF(LED_OPT[0].pLEDPort);
			LED_OFF(LED_OPT[1].pLEDPort);
			LED_OFF(LED_OPT[2].pLEDPort);			
		}break;
		default:break;
	}
}

/*******************************************************************************
函数名:	 static void LED_OFF_OPT_Func(void)
功能说明：	LED1关
参	数：	无
返回值：	无
*******************************************************************************/
static void LED_OFF_OPT_Func(void)
{
	switch(gled_off_opt)
	{
		case LED_1_OFF:
		{
			LED_OFF(LED_OPT[0].pLEDPort);
		}break;
		case LED_2_OFF:
		{
			LED_OFF(LED_OPT[1].pLEDPort);
		}break;
		case LED_3_OFF:
		{
			LED_OFF(LED_OPT[2].pLEDPort);
		}break;
		default:break;
	}
}

/*******************************************************************************
函数名:		void LED_Flash_Func(LED_ON_t led_on_opt,u8 keynum,LED_OFF_t led_off)
功能说明：	用于按键背光闪烁
参	数：	led_on_opt:按键灯亮的类型参数（几盏灯亮）
			keynum：当前有效按键号
			led_off：当前关灯类型参数
返回值：	无
*******************************************************************************/
void LED_Flash_Func(LED_ON_t led_on_opt,u8 keynum,LED_OFF_t led_off)
{
	gKeyNum       = keynum;
	gled_off_opt  = (LED_OFF_t)keynum;
	gled_off_Anti_opt = led_off;
	LED_ON_OPT_Func(led_on_opt);
    Thread_Login(MANY, 12, 100, LED_RVRS_OPT_Func);
    Thread_Login(ONCEDELAY, 0, 30000, LED_OFF_OPT_Func);
	Thread_Login(ONCEDELAY, 0, 20000, LED_OFF_Anti_OPT_Func);
}

/*******************************************************************************
函数名:		void LED_All_RVRS(void)
功能说明：	全部LED动作取反
参	数：	无
返回值：	无
*******************************************************************************/
void LED_All_RVRS(void)
{
    LED_REV(LED_OPT[0].pLEDPort);
	LED_REV(LED_OPT[1].pLEDPort);
	LED_REV(LED_OPT[2].pLEDPort);
}

/*********************************End of File**********************************/

