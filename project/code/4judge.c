#include "4judge.h"
uint8 judge_uart_buffer[JUDGE_UART_BUFFER_SIZE];
uint8 class_uart_buffer[CLASS_UART_BUFFER_SIZE];
char ** result_queue;
void judge_init(void)
{

	 uart_init (UART_1,115200, UART1_TX_B12, UART1_RX_B13);
	 uart_rx_interrupt(UART_1,1);																						
	 interrupt_set_priority(LPUART1_IRQn, 0);
	 uart_init (UART_4,115200, UART4_TX_C16, UART4_RX_C17);
	 uart_rx_interrupt(UART_4,2);																						
	 interrupt_set_priority(LPUART4_IRQn, 0);

}



