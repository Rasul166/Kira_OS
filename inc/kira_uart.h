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
void kira_uart_init(void);
void kira_print_string(char *str);
void kira_print_char(char c);
struct rx_buffer{
  char ring_buffer[64]; //to store the received bits 
  volatile unsigned int head;//index position of ring buffer to where received data should be stored
  volatile unsigned int tail; //index position of ring buffer from where received data should be retrieved
  };
  extern struct rx_buffer rx_b1;
void ring_buffer_push(char data);
	  char ring_buffer_pop(void);
      void USART1_IRQHandler(void) ;