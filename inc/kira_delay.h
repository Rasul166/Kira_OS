#define ctrlreg *((volatile unsigned int *)0xE000E010)
#define loadreg *((volatile unsigned int *)0xE000E014)
#define currreg *((volatile unsigned int *)0xE000E018)
volatile unsigned int ticks = 0;

void delayms(int no_of_ms) ;
void SysTick_Handler(void) ;
void kira_delay_init(void);

