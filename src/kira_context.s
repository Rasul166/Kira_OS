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
LDR Ir,=0xFFFFFFFD
BX Ir
END