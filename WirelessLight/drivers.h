/*
 * drivers.h
 *
 * Created: 12/7/2014 8:48:12 PM
 *  Author: eric
 */ 


#ifndef DRIVERS_H_
#define DRIVERS_H_

#define 	RELAY PA6


void relay_on();
void relay_off();

void sound_start(int16_t freq, int16_t dur);
void lcd_sendtext(char *str);
void lcd_text(char *str);
void init_drivers();

#endif /* DRIVERS_H_ */