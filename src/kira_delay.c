#include<kira_delay.h>
#include<kira_task.h>   
volatile unsigned int ticks=0;

void delayms(int no_of_ms) {
    unsigned int start = ticks;
    while(ticks - start < no_of_ms) {}
}
void SysTick_Handler(void) {
    ticks++;
    kira_schedular();
}
void kira_delay_init(void){
 loadreg = 107999;
    currreg = 0;
    ctrlreg |= (7);
}