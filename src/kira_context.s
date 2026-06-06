	AREA |.text|,CODE,READONLY
	IMPORT current_task_pointer
	IMPORT next_task_pointer
	EXPORT PendSV_Handler
	EXPORT SVC_Handler
	
PendSV_Handler
	MRS R0,PSP
	STMDB R0!, {R4-R11}
	LDR R1,=current_task_pointer
	LDR R2,[R1]
	STR R0,[R2]
	LDR R1,=next_task_pointer
	LDR R2,[R1]
	LDMIA R0!,{R4-R11}
	MSR PSP,R0

	LDR R1, =next_task_pointer      ; Get the address of next_task_pointer
    LDR R2, [R1]                    ; Read the TCB address inside it
    LDR R3, =current_task_pointer   ; Get the address of current_task_pointer
    STR R2, [R3]                    ; Overwrite current_task with next_task
	LDR LR,=0xFFFFFFFD
	BX LR

SVC_Handler
	LDR R1,=current_task_pointer
	LDR R2,[R1]
	LDR R0,[R2]
	LDMIA R0!,{R4-R11}

	MSR PSP,R0


	MOV R0,#2
	MSR CONTROL,R0 
	LDR LR,=0xFFFFFFFD
	BX LR
	END