#include <stdint.h>

#define ctrlreg     *((volatile unsigned int *)0xE000E010)
#define loadreg     *((volatile unsigned int *)0xE000E014)
#define currreg     *((volatile unsigned int *)0xE000E018)
#define rcc_apb2enr *((volatile unsigned int *)0x40021018)
#define gpio_crh    *((volatile unsigned int *)0x40011004)
#define gpio_odr    *((volatile unsigned int *)0x4001100C)

volatile unsigned int ticks = 0;

void SysTick_Handler(void){
    ticks++;
}

void delayms(unsigned int no_of_ms){
    unsigned int start = ticks;
    while ((ticks - start) < no_of_ms) {
    }
}

int main(){
    loadreg = 7999;
    currreg = 0;
    ctrlreg = 0x07;

    rcc_apb2enr |= (1<<4);
    gpio_crh &= ~(0xF<<20);
    gpio_crh |= (0x2<<20);

    while(1) {
        gpio_odr ^= (1<<13);
        delayms(500);
    }

    return 0;
}