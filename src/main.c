#include <stdio.h>
#include <kira_uart.h>
#include<kira_delay.h>
#include<kira_task.h>
// Reset and Clock Control
#define rcc_apb2enr *((volatile unsigned int *)0x40021018)

// GPIO Port C (LED Blinking)
#define gpio_crh *((volatile unsigned int *)0x40011004)
#define gpio_odr *((volatile unsigned int *)0x4001100C)
  
	volatile int current_task=0;
     volatile int task_count=0;
	



int main() {
    // 1. Initialize SysTick (1ms heartbeat at 72MHz / 8)
   kira_delay_init();
    
    // 2. Initialize Port C Clock for LED (Bit 4)
    rcc_apb2enr |= (1<<4);
    
    // 3. Configure PC13 for LED
    gpio_crh &= ~(0xF<<20);
    gpio_crh |= (0x2<<20);
    
    
    
    // 5. Initialize UART & NVIC
    kira_uart_init();
    
    // 6. Main OS Loop
    while(1) {
       kira_print_string("created a uart stuff basic \r\n"); 
       while(rx_b1.head!=rx_b1.tail) //checking if the ring buffer is not empty
       {
			 char ba=ring_buffer_pop();
			 kira_print_char(ba);
			 }
			 //retrieving data from buffer and then printing
    delayms(1000);
    }
    
    return 0;
}
