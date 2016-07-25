/***************************Copyright BestFu 2016-04-13*************************
文	件：	Thread.h
说	明：	线程管理头文件
编	译：	uVision V5.12.0.0
版	本：	v1.0
编	写：	jay
日	期：	2014.06.18  
修　改：	暂无
*******************************************************************************/
#ifndef __THREAD_H
#define __THREAD_H

/*************************************自定义宏*********************************/
#define THREAD_TIME_SLICE  		(1u)	//线程时间片大小,1ms

/**********************************数据结构定义********************************/
typedef struct
{
	BF_INT08U	run_con;	//运行条件
	BF_INT08U 	flag;		//标记运行标记
	BF_INT16U	run_cnt;	//运行次数
	BF_INT32U	run_frep;	//运行频率
	CPU_TS_TMR	tick_ctr; 	//心跳计数
	BF_FNCT_PTR fun;		//运行函数指针	
}Thread_t;

typedef struct
{
	BF_INT16U	runCnt;		//当前线程运行数
	BF_INT16U	hangCnt;	//当前线程挂起数
    Thread_t thread[MAX_THREAD_NUM];
}ThreadManage_t;

typedef enum
{
    ONCEDELAY	= 0,    //单次延时运行
    MANY		= 1,   	//多次
    FOREVER     = 2,    //永久
    RESULT      = 3,    //按结果
}ThreadCon_t;     		//线程驻存条件
 
typedef enum
{
	BF_ERR_THREAD_NONE = 1,		//线程首次注册
	BF_ERR_THREAD_EXHAUSTED,	//当前线程已满
	BF_ERR_THREAD_EXIST,		//线程已存在
}Thread_ERR_t;
 
/**********************************函数声明************************************/
void Thread_Init(void);     //线程初始化函数
Thread_ERR_t Thread_Login(ThreadCon_t con, BF_INT16U cnt, BF_INT32U frep, void * fun);   //线程注册
void Thread_Logout(void *fun);	//线程注销
void Thread_Process(void);		//线程运行
void Thread_RunCheck(void); 		//线程运行时间核对  
void Thread_Hang(void * fun);
void Thread_Wake(void * fun);

#endif //Thread.h end
/**************************Copyright BestFu 2016-04-13*************************/
