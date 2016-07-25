/***************************Copyright BestFu 2014-05-14*************************
文	件：	Thread.c
说	明：	线程实现函数
编	译：	Keil uVision4 V4.54.0.0
版	本：	v1.0
编	写：	Unarty
日	期：	2014.06.18
修　改：	2014.08.30 //修改单次延时注册线程，延时零毫秒不能执行问题
*******************************************************************************/
#include "lib_type.h"
#include "lib_config.h"
#include "BestFuLib.h"
#include "Thread.h"

//Private variable define
#define THREAD_TOTAL    (thread.runCnt + thread.hangCnt)    	//运行线程总数

//define Private variable
static ThreadManage_t   thread;

//Private function declaration
static BF_INT32U Thread_Check(BF_INT32U start, BF_INT32U end, void *fun);

/*******************************************************************************
函数  名:  Thread_Init
功能说明:  线程数据初始化
参    数:  无
返回  值:  无
*******************************************************************************/
void Thread_Init(void)
{
    *(BF_INT32U*)&thread.runCnt = 0;   //运行线程总数置空
}

/*******************************************************************************
函数  名:  Thread_Login
功能说明:  线程注册
参    数:  fun: 执行函数的指针
           执行描述
返回  值:  注册结果
*******************************************************************************/
Thread_ERR_t Thread_Login(ThreadCon_t con,BF_INT16U cnt,BF_INT32U frep, void * fun)
{
    BF_INT32U pos = 0u;
    Thread_ERR_t err = BF_ERR_THREAD_NONE;

    ERRR(THREAD_TOTAL >= MAX_THREAD_NUM, return BF_ERR_THREAD_EXHAUSTED); 			//线程空间
    if (0 == Thread_Check(0, THREAD_TOTAL, fun))  									//线程没有出现在线程表中
    {

        memmove(&thread.thread[thread.runCnt+1], &thread.thread[thread.runCnt],   //空闲一个空间，给新建线程
                sizeof(Thread_t)*(thread.hangCnt));
        pos = thread.runCnt;
        thread.runCnt++;
    }
    else
    {
        Thread_Wake(fun);	//线程换醒
        pos = Thread_Check(0, THREAD_TOTAL, fun) - 1;
        err = BF_ERR_THREAD_EXIST;
    }
    thread.thread[pos].run_con = con;
    thread.thread[pos].fun = (BF_FNCT_PTR)fun;

    if(ONCEDELAY == con)  															//单次延时线程
    {
        frep = ((frep > 0u)?(frep):(1u));
        cnt = 1;
    }
    thread.thread[pos].flag = 0;
    thread.thread[pos].run_cnt = cnt;
    thread.thread[pos].run_frep = frep;
    thread.thread[pos].tick_ctr = 0u;

    return (err);
}

/*******************************************************************************
函数  名:  Thread_Logout
功能说明:  线程注销
参    数:  fun: 执行函数的指针
返回  值:  无
*******************************************************************************/
void Thread_Logout(void * fun)
{
    BF_INT32U pos = Thread_Check(0, THREAD_TOTAL, fun);

    if (pos)      													//找到线程位置
    {
        if(pos < THREAD_TOTAL)
        {
            memmove(&thread.thread[pos-1], &thread.thread[pos],	//若删去的任务在任务队列的队中,则后面的任务前移
                    sizeof(Thread_t)*(THREAD_TOTAL - pos));
        }
        thread.runCnt--;
    }
}

/*******************************************************************************
函数  名:  Thread_Hang
功能说明:  线程挂起
参    数:  fun: 执行函数的指针
返回  值:  无
*******************************************************************************/
void Thread_Hang(void * fun)
{
    BF_INT32U i = Thread_Check(0, thread.runCnt, fun);

    if (i)     //线程位于运行线程空间
    {

        Thread_t th;

        memcpy(&th, &thread.thread[i-1], sizeof(Thread_t));
        memmove(&thread.thread[i-1], &thread.thread[i],
                sizeof(Thread_t)*(thread.runCnt - i));
        memcpy(&thread.thread[--thread.runCnt], &th, sizeof(Thread_t));
        thread.hangCnt++;
    }
}

/*******************************************************************************
函数  名:  Thread_Wake
功能说明:  线程换醒
参    数:  fun: 执行函数的指针
返回  值:  无
注    意:  本功能只能对已有线程的操作
*******************************************************************************/
void Thread_Wake(void * fun)
{
    BF_INT32U i = Thread_Check(thread.runCnt, THREAD_TOTAL, fun);

    if (i)     //找到线程位置
    {

        if ((i - thread.runCnt) > 1)     //i不是紧靠运行线程
        {

            Thread_t th;
            memcpy(&th, &thread.thread[i-1], sizeof(Thread_t));
            memmove(&thread.thread[thread.runCnt + 1], &thread.thread[thread.runCnt], //腾出存储空间
                    sizeof(Thread_t)*(i - thread.runCnt));
            memcpy(&thread.thread[thread.runCnt], &th,  sizeof(Thread_t));
        }
        thread.runCnt++;
        thread.hangCnt--;
    }
}

/*******************************************************************************
函数  名:  Thread_Process
功能说明:  线程处理
参    数:  无
返回  值:  无
*******************************************************************************/
void Thread_Process(void)
{
    Thread_t *pThread = NULL;
    void* fun = NULL;

    CPU_SR_ALLOC();
    for (pThread = &thread.thread[0]; pThread < &thread.thread[thread.runCnt]; pThread++)
    {
        if (pThread->flag)
        {

            ENTER_CRITICAL();
            fun = pThread->fun;
            pThread->flag = 0;
            switch (pThread->run_con)
            {
            case ONCEDELAY:
                pThread->flag = 1;
            case MANY:
            {
                if (!pThread->run_cnt --)
                {

                    Thread_Logout(fun);
                    pThread --;
                    EXIT_CRITICAL();
                    break;
                }
            }
            case FOREVER:
            {

                EXIT_CRITICAL();
                ((BF_FNCT_VOID)(fun))();
            }
            break;
            case RESULT:
            {
                EXIT_CRITICAL();
                if (0 == ((BF_FNCT_PTR)(fun))())   //线程运行返回失败
                {

                    Thread_Logout(fun);
                    pThread --;
                }
            }
            break;
            default :
            {

                EXIT_CRITICAL();
            }
            break;
            }
        }
    }
}

/*******************************************************************************
函数  名:  Thread_RunCheck
功能说明:  线程运行条件核对
参    数:  无
返回  值:  无
*******************************************************************************/
void Thread_RunCheck(void)
{
    Thread_t *pThread = NULL;
    CPU_SR_ALLOC();

    ENTER_CRITICAL();
    for (pThread = &thread.thread[0]; pThread < &thread.thread[thread.runCnt]; pThread++)
    {

        if ((++pThread->tick_ctr) >= (pThread->run_frep))
        {

            pThread->tick_ctr = 0;
            pThread->flag = 1;
        }
    }
    EXIT_CRITICAL();
}

/*******************************************************************************
函数  名:   Thread_Check
功能说明:   线程核对
参    数:   fun:     线程名
            start:  起始线程位置
            end:    终止线程位置
返回  值:   0(没有)/(1~THREAD_UNM)线程所处位置
*******************************************************************************/
static BF_INT32U Thread_Check(BF_INT32U start, BF_INT32U end, void *fun)
{
    BF_INT32U i;

    for (i = start; i < end;)
    {

        if (thread.thread[i++].fun == fun)
        {

            return i;
        }
    }
    return 0;
}

/**************************Copyright BestFu 2014-05-14*************************/

