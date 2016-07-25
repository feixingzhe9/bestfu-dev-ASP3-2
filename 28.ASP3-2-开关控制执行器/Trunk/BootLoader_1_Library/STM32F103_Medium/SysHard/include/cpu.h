#ifndef CPU_H
#define CPU_H
/*
*********************************************************************************************************
*                                    CONFIGURE STANDARD DATA TYPES
*
* Note(s) : (1) Configure standard data types according to CPU-/compiler-specifications.
*
*           (2) (a) (1) 'CPU_FNCT_VOID' data type defined to replace the commonly-used function pointer
*                       data type of a pointer to a function which returns void & has no arguments.
*
*                   (2) Example function pointer usage :
*
*                           CPU_FNCT_VOID  FnctName;
*
*                           FnctName();
*
*               (b) (1) 'CPU_FNCT_PTR'  data type defined to replace the commonly-used function pointer
*                       data type of a pointer to a function which returns void & has a single void
*                       pointer argument.
*
*                   (2) Example function pointer usage :
*
*                           CPU_FNCT_PTR   FnctName;
*                           void          *p_obj
*
*                           FnctName(p_obj);
*********************************************************************************************************
*/
typedef            void        CPU_VOID;
typedef            char        CPU_CHAR;                        /*  8-bit character                                     */
typedef  unsigned  char        CPU_BOOLEAN;                     /*  8-bit boolean or logical                            */
typedef  unsigned  char        CPU_INT08U;                      /*  8-bit unsigned integer                              */
typedef    signed  char        CPU_INT08S;                      /*  8-bit   signed integer                              */
typedef  unsigned  short       CPU_INT16U;                      /* 16-bit unsigned integer                              */
typedef    signed  short       CPU_INT16S;                      /* 16-bit   signed integer                              */
typedef  unsigned  int         CPU_INT32U;                      /* 32-bit unsigned integer                              */
typedef    signed  int         CPU_INT32S;                      /* 32-bit   signed integer                              */
typedef  unsigned  long  long  CPU_INT64U;                      /* 64-bit unsigned integer                              */
typedef    signed  long  long  CPU_INT64S;                      /* 64-bit   signed integer                              */

typedef            float       CPU_FP32;                        /* 32-bit floating point                                */
typedef            double      CPU_FP64;                        /* 64-bit floating point                                */


typedef  volatile  CPU_INT08U  CPU_REG08;                       /*  8-bit register                                      */
typedef  volatile  CPU_INT16U  CPU_REG16;                       /* 16-bit register                                      */
typedef  volatile  CPU_INT32U  CPU_REG32;                       /* 32-bit register                                      */
typedef  volatile  CPU_INT64U  CPU_REG64;                       /* 64-bit register                                      */

/*
*********************************************************************************************************
*                                   CRITICAL SECTION CONFIGURATION
*
* Note(s) : (1) Configure CPU_CFG_CRITICAL_METHOD with CPU's/compiler's critical section method :
*
*                                                       Enter/Exit critical sections by ...
*
*                   CPU_CRITICAL_METHOD_INT_DIS_EN      Disable/Enable interrupts
*                   CPU_CRITICAL_METHOD_STATUS_STK      Push/Pop       interrupt status onto stack
*                   CPU_CRITICAL_METHOD_STATUS_LOCAL    Save/Restore   interrupt status to local variable
*
*               (a) CPU_CRITICAL_METHOD_INT_DIS_EN  is NOT a preferred method since it does NOT support
*                   multiple levels of interrupts.  However, with some CPUs/compilers, this is the only
*                   available method.
*
*               (b) CPU_CRITICAL_METHOD_STATUS_STK    is one preferred method since it supports multiple
*                   levels of interrupts.  However, this method assumes that the compiler provides C-level
*                   &/or assembly-level functionality for the following :
*
*                     ENTER CRITICAL SECTION :
*                       (1) Push/save   interrupt status onto a local stack
*                       (2) Disable     interrupts
*
*                     EXIT  CRITICAL SECTION :
*                       (3) Pop/restore interrupt status from a local stack
*
*               (c) CPU_CRITICAL_METHOD_STATUS_LOCAL  is one preferred method since it supports multiple
*                   levels of interrupts.  However, this method assumes that the compiler provides C-level
*                   &/or assembly-level functionality for the following :
*
*                     ENTER CRITICAL SECTION :
*                       (1) Save    interrupt status into a local variable
*                       (2) Disable interrupts
*
*                     EXIT  CRITICAL SECTION :
*                       (3) Restore interrupt status from a local variable
*
*           (2) Critical section macro's most likely require inline assembly.  If the compiler does NOT
*               allow inline assembly in C source files, critical section macro's MUST call an assembly
*               subroutine defined in a 'cpu_a.asm' file located in the following software directory :
*
*                   \<CPU-Compiler Directory>\<cpu>\<compiler>\
*
*                       where
*                               <CPU-Compiler Directory>    directory path for common   CPU-compiler software
*                               <cpu>                       directory name for specific CPU
*                               <compiler>                  directory name for specific compiler
*
*           (3) (a) To save/restore interrupt status, a local variable 'cpu_sr' of type 'CPU_SR' MAY need
*                   to be declared (e.g. if 'CPU_CRITICAL_METHOD_STATUS_LOCAL' method is configured).
*
*                   (1) 'cpu_sr' local variable SHOULD be declared via the CPU_SR_ALLOC() macro which, if
*                        used, MUST be declared following ALL other local variables.
*
*                        Example :
*
*                           void  Fnct (void)
*                           {
*                               CPU_INT08U  val_08;
*                               CPU_INT16U  val_16;
*                               CPU_INT32U  val_32;
*                               CPU_SR_ALLOC();         MUST be declared after ALL other local variables
*                                   :
*                                   :
*                           }
*
*               (b) Configure 'CPU_SR' data type with the appropriate-sized CPU data type large enough to
*                   completely store the CPU's/compiler's status word.
*********************************************************************************************************
*/
typedef  CPU_INT32U                 CPU_SR;                     /* Defines   CPU status register size (see Note #3b).   */
                                                         
#ifndef   CPU_CRITICAL_METHOD_STATUS_LOCAL
#define  CPU_SR_ALLOC()             CPU_SR  cpu_sr = (CPU_SR)0
#else
#define  CPU_SR_ALLOC()
#endif

/*
*********************************************************************************************************
*                                      CPU TIMESTAMP DATA TYPES
*
* Note(s) : (1) CPU timestamp timer data type defined to the binary-multiple of 8-bit octets as configured
*               by 'CPU_CFG_TS_TMR_SIZE' (see 'cpu_cfg.h  CPU TIMESTAMP CONFIGURATION  Note #2').
*********************************************************************************************************
*/
typedef  CPU_INT32U  CPU_TS32;
typedef  CPU_INT64U  CPU_TS64;
typedef  CPU_TS32    CPU_TS;                                    /* Req'd for backwards-compatibility.                   */                                                        
typedef  CPU_INT32U  CPU_TS_TMR;								/* CPU ts tmr dflt size = 32-bits.                      */

#define  ENTER_CRITICAL()         do { cpu_sr = CPU_SR_Save(); } while (0) /* Save    CPU status word & disable interrupts.*/
#define  EXIT_CRITICAL()          do { CPU_SR_Restore(cpu_sr); } while (0) /* Restore CPU status word.                     */

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/
CPU_SR CPU_SR_Save(void);
void CPU_SR_Restore(CPU_SR cpu_sr);
void WFI_SET(void);
void Set_MSP(CPU_INT32U addr);
void SystemReset(void);

#endif
/***************************************************End of File*****************************************************/



