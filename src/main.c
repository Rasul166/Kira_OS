#include <stdio.h>
#include <kira_uart.h>
#include<kira_delay.h>
#include<kira_task.h>
// Reset and Clock Control
#define rcc_apb2enr *((volatile unsigned int *)0x40021018)

// GPIO Port C (LED Blinking)
#define gpio_crh *((volatile unsigned int *)0x40011004)
#define gpio_odr *((volatile unsigned int *)0x4001100C)
Mutex_t uart_mutex;
	
void Task1(void){
	while(1){
	kira_mutex_take(&uart_mutex);
    kira_print_string("1111111111111111111111111111 end ");
kira_mutex_give(&uart_mutex);
		kira_task_sleep(6);
	}
		
	
		delayms(1);
	}

void Task2(void){
	 while(1){
		kira_mutex_take(&uart_mutex);
    kira_print_string("222222222222222222222222222 end");
	 kira_mutex_give(&uart_mutex);
		   kira_task_sleep(2);
	 }
  
		delayms(1);
	 }


int main() {
    // 1. Initialize SysTick (1ms heartbeat at 72MHz / 8)
	kira_uart_init();
	kira_task_create(Task1);
	kira_task_create(Task2);

	kira_systick_init();
	kira_mutex_init(&uart_mutex);
	kira_os_start();
    return 0;
}
