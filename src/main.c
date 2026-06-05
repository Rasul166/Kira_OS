#include <stdio.h>
#include <kira_uart.h>
#include<kira_delay.h>
#include<kira_task.h>
// Reset and Clock Control
#define rcc_apb2enr *((volatile unsigned int *)0x40021018)

// GPIO Port C (LED Blinking)
#define gpio_crh *((volatile unsigned int *)0x40011004)
#define gpio_odr *((volatile unsigned int *)0x4001100C)
  
void Task1(void){
    kira_print_string("Tick");
    kira_task_sleep(1000);
}
void Task2(void){
    kira_print_string("Tock");
    kira_task_sleep(500);
}
int main() {
    // 1. Initialize SysTick (1ms heartbeat at 72MHz / 8)

	kira_task_create(Task1);
	  kira_task_create(Task2);
		kira_os_start();
	kira_delay_init();
    
    return 0;
}
