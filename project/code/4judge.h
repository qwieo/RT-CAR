#ifndef __4JUDGE__H__
#define __4JUDGE__H__
#include "zf_common_headfile.h"
#define JUDGE_UART_BUFFER_SIZE 32
#define CLASS_UART_BUFFER_SIZE 32
extern uint8 judge_uart_buffer[JUDGE_UART_BUFFER_SIZE];
extern uint8 class_uart_buffer[CLASS_UART_BUFFER_SIZE];
extern char ** result_queue;
void judge_init(void);
#endif 
