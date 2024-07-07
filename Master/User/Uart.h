#ifndef UART_H
#define UART_H
#include "stm32f10x.h"
#include <stdio.h>

#define Debug printf

//void COM1_Init( void);
void COM1_2_Init( void);
extern uint16_t Instruction;

#define UART1_RX_BUF_LEN  30

#define UART2_RX_BUF_LEN  30

#endif
