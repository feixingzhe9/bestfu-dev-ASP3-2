;/* 	2012-9  Created by dezhi jiang
; *	xxxxxx   please added here
; */

    EXPORT  CPU_SR_Save    
    EXPORT  CPU_SR_Restore
	EXPORT	WFI_SET
	EXPORT	Set_MSP
	EXPORT  SystemReset


;********************************************************************************************************
;                                      CODE GENERATION DIRECTIVES
;********************************************************************************************************

    AREA |.text|, CODE, READONLY, ALIGN=2
    THUMB
    REQUIRE8
    PRESERVE8

CPU_SR_Save
    MRS     R0, PRIMASK     
    CPSID   I
    BX      LR

CPU_SR_Restore
    MSR     PRIMASK, R0
    BX      LR
	
WFI_SET
	WFI
	
Set_MSP 
    MSR 	MSP, r0 	  		;set Main Stack value
    BX  	R14
	
SystemReset
	MOV 	R0, #1           
	MSR 	FAULTMASK, R0   	;清除FAULTMASK禁止一切中断
	LDR 	R0, =0xE000ED0C  
	LDR 	R1, =0x05FA0004 
	STR 	R1, [R0]        	;系统软件复位   
	NOP
	
deadloop
    B 		deadloop        	;等待系统复位
		
    END
