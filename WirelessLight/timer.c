/*
 * timer.c
 *
 * Created: 12/7/2014 7:33:13 PM
 *  Author: eric
 */ 
#include "main.h"

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "timer.h"

volatile uint32_t seconds = 0;

#define DATEBUFF_LEN 17
char datebuff[DATEBUFF_LEN];

ISR(TIMER1_COMPA_vect) {
	seconds++;
	TCNT1 = 0;
}

void init_timer() {
	// set 1024 prescaler
	TCCR1B = _BV(CS12) | _BV(CS10);
	 
	OCR1A = (F_CPU / 1024);
	TIMSK = _BV(OCIE1A);
}

char *ctime() {
	uint32_t acc = seconds;

	uint16_t year = 1970;
	while (1) {
		if ISLEAP(year) {
			if (acc > LYEAR_SEC) { 
				year++;
				acc -= LYEAR_SEC;
			} else {
				break;
			}
		} else {
			if (acc > YEAR_SEC) {
				year++;
				acc -= YEAR_SEC;
			} else {
				break;
			}
		}
	}
		
	char *month = NULL;
	while (1) {
		if (acc < DAYS_31) {
			month = "Jan";
			break;
		} else {
			acc -= DAYS_31;
		}
		if (ISLEAP(year)) {
			if (acc < DAYS_29) {
				month = "Feb";
				break;
			} else {
				acc -= DAYS_29;
			}
		} else {
			if (acc < DAYS_28) {
				month = "Feb";
				break;
			} else {
				acc -= DAYS_28;
			}		
		}
		if (acc < DAYS_31) {
			month = "Mar";
			break;
		} else {
			acc -= DAYS_31;
		}
		if (acc < DAYS_30) {
			month = "Apr";
			break;
		} else {
			acc -= DAYS_30;
		}
		if (acc < DAYS_31) {
			month = "May";
			break;
		} else {
			acc -= DAYS_31;
		}
		if (acc < DAYS_30) {
			month = "Jun";
			break;
		} else {
			acc -= DAYS_30;
		}
		if (acc < DAYS_31) {
			month = "Jul";
			break;
		} else {
			acc -= DAYS_31;
		}
		if (acc < DAYS_31) {
			month = "Aug";
			break;
		} else {
			acc -= DAYS_31;
		}
		if (acc < DAYS_30) {
			month = "Sep";
			break;
		} else {
			acc -= DAYS_30;
		}
		if (acc < DAYS_31) {
			month = "Oct";
			break;
		} else {
			acc -= DAYS_31;
		}
		if (acc < DAYS_30) {
			month = "Nov";
			break;
		} else {
			acc -= DAYS_30;
		}
		month = "Dec";
		break;
	}
	
	
	uint8_t day=1;
	while (acc > DAY_1) {
		acc -= DAY_1;
		day++;
	}
	
	uint8_t hour=0;
	while (acc > 60 * 60) {
		acc -= 60*60;
		hour++;
	}	
	
	uint8_t minute=0;
	while (acc > 60) {
		acc -= 60;
		minute++;
	}
	
	uint8_t sec=acc;
	
	snprintf(datebuff, sizeof(datebuff), "%s %02hhu %02hhu:%02hhu:%02hhu", month, day, hour, minute, sec);
	return datebuff;
}

void printtime() {
	printf("[%s]", ctime());
}