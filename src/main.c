#include <stdio.h>

// SysTick Timer
#define ctrlreg *((volatile unsigned int *)0xE000E010)
#define loadreg *((volatile unsigned int *)0xE000E014)
#define currreg *((volatile unsigned int *)0xE000E018)

// Reset and Clock Control
#define rcc_apb2enr *((volatile unsigned int *)0x40021018)

// GPIO Port C (LED Blinking)
#define gpio_crh *((volatile unsigned int *)0x40011004)
#define gpio_odr *((volatile unsigned int *)0x4001100C)

// GPIO Port A (UART Pins)
#define porta_crh *((volatile unsigned int *)0x40010804)	

// UART1 Registers
#define status_reg *((volatile unsigned int *)0x40013800)
#define data_reg *((volatile unsigned int *)0x40013804)
#define baudrate_reg *((volatile unsigned int *)0x40013808)
#define control_1_reg *((volatile unsigned int *)0x4001380C)

// NVIC (Nested Vectored Interrupt Controller)
#define nvic_reg_iser1 *((volatile unsigned int *)0xE000E104)
	
volatile unsigned int ticks = 0;

void delayms(int no_of_ms) {
    unsigned int start = ticks;
    while(ticks - start < no_of_ms) {}
}

void kira_uart_init(void) {
    // Enable Clocks: GPIOA (Bit 2), USART1 (Bit 14), and AFIO (Bit 0)
    rcc_apb2enr |= ((1<<2) | (1<<14) | (1<<0));
    
    // Set Baudrate
    baudrate_reg = 0x0045;
    
    // Enable USART (Bit 13), TX (Bit 3), RXNE Interrupt (Bit 5), and RX (Bit 2)
    control_1_reg |= (1<<13) | (1<<3) | (1<<5) | (1<<2);
    
    // Enable USART1 Global Interrupt in NVIC (Position 37 -> ISER1, Bit 5)
    nvic_reg_iser1 |= (1<<5);
    
    
}

void kira_print_char(char c) {
    // Wait until Transmit Data Register is empty (TXE, Bit 7)
    while(((1<<7) & status_reg) == 0) {}
    data_reg = c;
}

void kira_print_string(char *str) {
    while(*str != '\0') {
        kira_print_char(*str);	
        str++;
    }
}

// OS Heartbeat
void SysTick_Handler(void) {
    ticks++;
}

// Asynchronous UART RX Interrupt Service Routine
void USART1_IRQHandler(void) {
    // If RX Not Empty (RXNE, Bit 5) is set
    if((status_reg & (1<<5))) {
        // Read character (automatically clears the interrupt flag)
        char c = (char)data_reg;
        // Echo character back to the terminal
        kira_print_char(c);
    }
}

int main() {
    // 1. Initialize SysTick (1ms heartbeat at 72MHz / 8)
    loadreg = 107999;
    currreg = 0;
    ctrlreg |= (7);
    
    // 2. Initialize Port C Clock for LED (Bit 4)
    rcc_apb2enr |= (1<<4);
    
    // 3. Configure PC13 for LED
    gpio_crh &= ~(0xF<<20);
    gpio_crh |= (0x2<<20);
    
    // 4. Configure PA9 (TX) as Alternate Function Push-Pull
    porta_crh &= ~(0xF<<4);
    porta_crh |= (0xB<<4);
    
    // 5. Initialize UART & NVIC
    kira_uart_init();
    
    // 6. Main OS Loop
    while(1) {
       kira_print_string("created a uart stuff basic"); 
        delayms(1000);
    }
    
    return 0;
}