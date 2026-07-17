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

#endif