
#include<kira_uart.h>
#include<stdint.h>
#include<kira_delay.h>
struct rx_buffer rx_b1={.head=0,.tail=0};

void ring_buffer_push(char data)
	{  
	 //checking if the buffer is full 
	  if((rx_b1.head+1)%64!=rx_b1.tail){
	  rx_b1.ring_buffer[rx_b1.head]=data; //adding the data to buffer if it is not full
	  rx_b1.head=(rx_b1.head+1)%64; //incrementing the head
	  }}
      char ring_buffer_pop(void)
	  {
	    // condition for empty buffer is checked in the main then comes to this function
	    char ab=rx_b1.ring_buffer[rx_b1.tail]; //taking data from the buffer and storing in ab
	    rx_b1.tail=(rx_b1.tail+1)%64; //incrementing the tail
	    return ab; //returning ab
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
    porta_crh &= ~(0xF<<4);
    porta_crh |= (0xB<<4);
    
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
void USART1_IRQHandler(void) {
    // If RX Not Empty (RXNE, Bit 5) is set
    if((status_reg & (1<<5))) {
        // Read character (automatically clears the interrupt flag)
        char c = (char)data_reg;
        // Echo character back to the terminal
        ring_buffer_push(c);
    }
}
void kira_print_time_and_text(char* text) {
    char time_str[12]; 
   unsigned int temp = ticks;
    int i = 0;
    
    // 1. If time is 0, handle it directly
    if (temp == 0) {
        time_str[i++] = '0';
    } 
    // 2. Extract digits backwards (e.g., 1024 becomes '4','2','0','1')
    else {
        while (temp > 0) {
            time_str[i++] = (temp % 10) + '0'; // Convert number to ASCII character
            temp /= 10;
        }
    }
    kira_print_string(text);
    // 3. Print the brackets: "["
    kira_print_string("[");
    
    // 4. Print the numbers in the correct forward order
    while (i > 0) {
        i--;
        // Send a single character to UART (assuming you have a character print function)
        // If you only have kira_print_string, you can make a temporary 2-char array:
        char single_char_str[2] = {time_str[i], '\0'};
        kira_print_string(single_char_str); 
    }
    
    // 5. Print the closing bracket and the user's text
    kira_print_string("] \n");
    
}
