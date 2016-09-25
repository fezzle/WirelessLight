/*
 * WirelessLight.c
 *
 * This code is for an Olimex ATmega128 devkit:
 * https://www.olimex.com/Products/AVR/Development/AVR-MT128/
 *
 * The configuration has the following properties:
 * - RS232 at 4800 baud
 * - Terminal commands accepted
 *   light on
 *   light off
 *   text <msg>
 *	 time <epochetime>    
 */ 
#include "main.h"

#include <string.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "drivers.h"
#include "uart.h"
#include "timer.h"
#include "command.h"


char command_buffer[RX_BUFFER_SIZE];
volatile uint8_t command_ready;
extern uint32_t seconds;
extern FILE uart_io;

extern char rx_buffer[];
extern uint8_t rx_head;
extern uint8_t rx_tail;


extern uint8_t rx_frame_errors;
extern uint8_t rx_overruns;
extern uint8_t rx_overflows;
extern char light;
extern char currenttext[];

int main(void) {
	init_uart();
	init_timer();
	init_drivers();
	sei();
	
	DDRA = _BV(RELAY);
	
	stdout = &uart_io;
	stdin  = &uart_io;

	char input = 'a';

	while(1) {
		printf("[%s] light[%d] text[%s]\n", ctime(), light, currenttext);	
		for (char i=0; i<250; i++) {
			if (command_ready) {
				gets(command_buffer);
				handle_command(command_buffer);
			} 
			_delay_ms(20);
		}
	}
}