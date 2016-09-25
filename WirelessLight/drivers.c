/*
 * drivers.c
 *
 * Created: 12/7/2014 8:47:58 PM
 *  Author: eric
 */ 

#include "main.h"
#include <util/delay.h>
#include <stdio.h>
#include <avr/io.h>

#include "drivers.h"
#include "timer.h"

#define		CLR_DISP	0x00000001
#define		DISP_ON		0x0000000C
#define		DISP_OFF	0x00000008
#define		CUR_HOME        0x00000002
#define		CUR_OFF 	0x0000000C
#define     CUR_ON_UNDER    0x0000000E
#define     CUR_ON_BLINK    0x0000000F
#define     CUR_LEFT        0x00000010
#define     CUR_RIGHT       0x00000014
#define		CUR_UP  	0x00000080
#define		CUR_DOWN	0x000000C0
#define     ENTER           0x000000C0
#define		DD_RAM_ADDR	0x00000080
#define		DD_RAM_ADDR2	0x000000C0

char currenttext[17] = {0};
char light = 0;

void lcd_pulse() {
	PORTC = PORTC | 0b00000100;	//set E to high
	_delay_us(5);				//delay ~110ms
	PORTC = PORTC & 0b11111011;	//set E to low
}


void lcd_sendcommand(char a) {
	_delay_ms(2);
	PORTC = (PORTC & 0b00001110) | (0b11110000 & a);		
	lcd_pulse();                              

	PORTC = (PORTC & 0b00001110) | (a<<4);	
	lcd_pulse();                              
}

void lcd_sendchar(char a) {
	_delay_ms(2);
	PORTC = (PORTC & 0b00001110) | (0b11110000 & a) | 1;		
	lcd_pulse();                              

	PORTC = (PORTC & 0b00001110) | (a<<4) | 1;					
	lcd_pulse();                            
}

void lcd_sendtext(char* t) {
	for (int i=0; i<16 && *t; i++) {
		lcd_sendchar(*t);
		t++;
	}
}

void init_drivers() {
	PORTC = 0b00000000;
	DDRC = 0b11110111;
	
	DDRE = 0b00110000;

	_delay_ms(110);
	
	//LCD initialization
	//step by step (from Gosho) - from DATASHEET
	PORTC = PORTC & 0b11111110;
		
	_delay_ms(110);

	PORTC = 0b00110000;						//set D4, D5 port to 1
	lcd_pulse();							//high->low to E port (pulse)
	_delay_ms(10);

	PORTC = 0b00110000;						//set D4, D5 port to 1
	lcd_pulse();							//high->low to E port (pulse)
	_delay_ms(10);

	PORTC = 0b00110000;						//set D4, D5 port to 1
	lcd_pulse();							//high->low to E port (pulse)
	_delay_ms(10);

	PORTC = 0b00100000;						//set D4 to 0, D5 port to 1
	lcd_pulse();							//high->low to E port (pulse)
	_delay_ms(1000);
	
	lcd_sendcommand(DISP_ON);		//Turn ON Display
	lcd_sendcommand(CLR_DISP);		//Clear Display
	lcd_sendtext("    Welcome     ");	//Print welcome screen
}


void relay_on() {
	light = 1;
	PORTA |= _BV(RELAY);
}
void relay_off() {
	light = 0;
	PORTA &= ~_BV(RELAY);
}

void sound_start(int16_t freq, int16_t dur) {
	unsigned int i;
	while(dur--) {
		PORTE = (PORTE & 0b11011111) | 0b00010000;	//set bit4 = 1; set bit5 = 0
		for (i=freq; i; i--);
		PORTE = (PORTE | 0b00100000) & 0b11101111;	//set bit4 = 0; set bit5 = 1
		for (i=freq; i; i--);
	}
}

void lcd_text(char *str) {
    lcd_sendcommand(CLR_DISP);
	lcd_sendcommand(DD_RAM_ADDR);
	lcd_sendtext(ctime());
	
	strncpy(currenttext, str, sizeof(currenttext)-1);
	currenttext[sizeof(currenttext) - 1] = 0;
	
	lcd_sendcommand(DD_RAM_ADDR2);
	lcd_sendtext(currenttext);
}
