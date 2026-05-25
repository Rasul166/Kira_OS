#include<kira_delay.h>
void delayms(int no_of_ms) {
    unsigned int start = ticks;
    while(ticks - start < no_of_ms) {}
}
void SysTick_Handler(void) {
    ticks++;
}
void kira_delay_init(void){
 loadreg = 107999;
    currreg = 0;
    ctrlreg |= (7);
}