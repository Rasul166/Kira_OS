#include<stdio.h>

// --- SysTick Registers ---
#define ctrlreg *((volatile unsigned int *)0xE000E010)
#define loadreg *((volatile unsigned int *)0xE000E014)
#define currreg *((volatile unsigned int *)0xE000E018)

// --- RCC & GPIO Registers ---
#define rcc_apb2enr *((volatile unsigned int *)0x40021018)
#define gpio_crh *((volatile unsigned int *)0x40011004)
#define gpio_odr *((volatile unsigned int *)0x4001100C)
#define porta_crh *((volatile unsigned int *)0x40010804)	

// --- USART1 Registers ---
#define status_reg *((volatile unsigned int *)0x40013800)
#define data_reg *((volatile unsigned int *)0x40013804)
#define baudrate_reg *((volatile unsigned int *)0x40013808)
#define control_1_reg *((volatile unsigned int *)0x4001380C)

volatile unsigned int ticks = 0;

void delayms(int no_of_ms) {
    unsigned int start = ticks;
    while ((ticks - start) < no_of_ms) {}
}

void kira_uart_init(void) {
    // 1. Enable APB2 clocks for Port A (Bit 2) and USART1 (Bit 14)
    rcc_apb2enr |= ((1 << 2) | (1 << 14));
    
    // 2. Configure PA9 as Alternate Function Output Push-Pull (0xB)
    porta_crh &= ~(0xF << 4);
    porta_crh |=  (0xB << 4);

    // 3. Set Baud Rate to 115200 at 8MHz
    baudrate_reg = 0x0045;
    
    // 4. Enable USART (Bit 13) and Transmitter (Bit 3)
    control_1_reg |= (1 << 13) | (1 << 3);
}

void kira_print_char(char c) {
    // Wait until TXE (Bit 7) is 1 (Hardware is ready for next character)
    while ((status_reg & (1 << 7)) == 0) {}
    
    // Shove the character into the hardware
    data_reg = c;
}

void kira_print_string(char *str) {
    // Iterate until we hit the null terminator
    while (*str != '\0') {
        kira_print_char(*str);	
        str++;
    }
}

void SysTick_Handler(void) {
    ticks++;
}

int main() {
    // SysTick Initialization
    loadreg = 107999;
    currreg = 0;
    ctrlreg |= 7;
    
    // Other GPIO Initialization 
    rcc_apb2enr |= (1 << 4);
    gpio_crh &= ~(0xF << 20);
    gpio_crh |= (0x2 << 20);
    
    kira_uart_init();
    
    while(1) {
        kira_print_string("successfully created a uart stuff basic\r\n");
        delayms(1000);
    }
    
    return 0;
}