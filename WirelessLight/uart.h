/*
 * IncFile1.h
 *
 * Created: 9/1/2014 5:04:59 PM
 *  Author: eric
 */ 


#ifndef UART_H_
#define UART_H_

#include <stdio.h>

void init_uart(void);
void uart_putchar(char c);
void uart_putstring(const char *s);
char uart_getchar();
void uart_puttime();

#endif /*UART_H_*/


