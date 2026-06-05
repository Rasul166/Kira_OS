AREA | .text|,CODE,READONLY
EXTERN current_task_pointer
EXTERN next_task_pointer
Export PendSV_Handler

PendSV_Handler
MRS R0,PSP
LDR R1,current_task_pointer
STR R0,[R1]
LDR R1,next_task_pointer
LDR R0,[R1]
LDMIA R0!,{R4-R11}
MSR PSP,R0
LDR lr,=0xFFFFFFFD
BX Ir

SVC_Handler
LDR R1,=current_task_pointer
LDR R2,[R1]
LDR R0,[R1]
LDMIA R0!,{R4-R11}
MSR PSP,R0

MOV R0,#2
MSR CONTROL,R0//Bit 1 = 1 means "Use PSP in Thread Mode(No privilages since it is just a task)"
LDR Lr,=0xFFFFFFFD
BX Lr
END