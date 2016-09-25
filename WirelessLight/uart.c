/*
 * Free as in Beer.
 */ 

#include "main.h"

#include <inttypes.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "uart.h"

volatile char rx_buffer[RX_BUFFER_SIZE];
volatile uint8_t rx_head = 0;
volatile uint8_t rx_tail = 0;
volatile uint8_t rx_lastchar = 0;
volatile uint16_t rx_overflows = 0;
volatile uint16_t rx_frame_errors = 0;
volatile uint16_t rx_overruns = 0;

volatile char tx_buffer[TX_BUFFER_SIZE];
volatile uint8_t tx_head = 0;
volatile uint8_t tx_tail = 0;
volatile uint8_t tx_inprogress = 0;

// Hook macro for STDIO
// process described in http://www.appelsiini.net/2011/simple-usart-with-avr-libc
FILE uart_io = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);

extern uint8_t command_ready;

/*
 * init_uart
 */
void init_uart(void) {
	// set baud rate
	UBRR1H = (uint8_t)(MYUBRR >> 8); 
	UBRR1L = (uint8_t)(MYUBRR);
	// +tramsmit, receive, receive interrupt
	UCSR1B = (1 << TXEN1) | (1 << RXEN1) | (1<< RXCIE1);
	// set frame format, asynchronous 8n1
	UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);	
}


/*
 * uart_putc
 * Puts a single char. Will block until there is enough space in the
 * send buffer.
 */
void uart_putchar(char c) {
  uint8_t tmp_head = (tx_head + 1) % TX_BUFFER_SIZE;
  // wait for space in buffer
  while (tmp_head == tx_tail) {
	;
  }
  tx_buffer[tx_head] = c;
  tx_head = tmp_head;
  // enable uart data interrupt (send data)
  UCSR1B |= (1<<UDRIE1);
  tx_inprogress = 1;
}


/*
 * uart_puts
 * Sends a string.
 */
void uart_putstring(const char *s) {
  while (*s) {
    uart_putchar(*s);
    s++;
  }
}

/*
 * Returns the next character in the receive buffer or 0 if buffer is empty.
 */
char uart_getchar() {
	if (rx_tail == rx_head) {
		if (command_ready > 0) {
			command_ready--;	
		}
		return '\n';
	} else {
		char res = rx_buffer[rx_tail];
		rx_tail = (rx_tail + 1) % (RX_BUFFER_SIZE);
		if (res == '\n') {
			command_ready--;
		}
		return res;
	}
}

/*
 * ISR User Data Register Empty
 * Send a char out of buffer via UART. If sending is complete, the 
 * interrupt gets disabled.
 */
ISR(USART1_UDRE_vect) {
  uint8_t tmp_tail = 0;
  if (tx_head != tx_tail) {
    tmp_tail = (tx_tail + 1) % TX_BUFFER_SIZE;
    UDR1 = tx_buffer[tx_tail];
    tx_tail = tmp_tail;
  } else {
    // disable this interrupt if nothing more to send
    UCSR1B &= ~(1 << UDRIE1);
	tx_inprogress = 0;
  }
}

/*
 * SIGNAL RX complete
 * Receives a char from UART and stores it in ring buffer.
 */
ISR(USART1_RX_vect) {
	if (UCSR1A & _BV(FE1)) {
		rx_frame_errors++;
	}

	if (UCSR1A & _BV(DOR1)) {
		rx_overruns++;
	}

	char data = UDR1;
	uint8_t tmp_head = (rx_head + 1) % RX_BUFFER_SIZE;
	
	if (data == '\n') {
		command_ready++;
	}
	
	if (tmp_head == rx_tail) {
		if (!command_ready) {
			command_ready = 1;
		}
		rx_overflows++;
	} else {
		rx_buffer[rx_head] = data;
		rx_head = tmp_head;
	}
	rx_lastchar = data;
}