#ifndef KIRA_REGISTERS_H
#define KIRA_REGISTERS_H

/* SysTick */

#define SYSTICK_CTRL     (*(volatile uint32_t *)0xE000E010)
#define SYSTICK_LOAD     (*(volatile uint32_t *)0xE000E014)
#define SYSTICK_VAL      (*(volatile uint32_t *)0xE000E018)

/* SCB */

#define SCB_ICSR         (*(volatile uint32_t *)0xE000ED04)
#define SCB_SHPR3        (*(volatile uint32_t *)0xE000ED20)

/* NVIC */

#define NVIC_ISER1       (*(volatile uint32_t *)0xE000E104)

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
#define rcc_apb2enr *((volatile unsigned int *)0x40021018)

// NVIC (Nested Vectored Interrupt Controller)
#define nvic_reg_iser1 *((volatile unsigned int *)0xE000E104)

#endif