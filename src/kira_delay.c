#include<kira_delay.h>
#include<kira_task.h>   
volatile unsigned int ticks=0;

void delayms(int no_of_ms) {
    unsigned int start = ticks;
    while(ticks - start < no_of_ms) {}
}
void SysTick_Handler(void) {
    ticks++;
    for(int i=0;i<sizeof(Task_table);i++){
       if(Task_table[i].state==TASK_SLEEPING){Task_table[i].sleep_ticks--;
        if(Task_table[i].sleep_ticks==0){
        Task_table[i].state=TASK_READY;
       }}

    }
    kira_schedular();
}
void kira_delay_init(void){
 loadreg = 107999;
    currreg = 0;
    ctrlreg |= (7);
}