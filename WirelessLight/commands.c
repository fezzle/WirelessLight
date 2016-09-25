/*
 * commands.c
 *
 * Created: 12/7/2014 8:31:57 PM
 *  Author: eric
 */ 
#include "main.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "command.h"
#include "drivers.h"
#include "timer.h"

uint16_t command_errors = 0;
extern uint32_t seconds;

void handle_command(char *commandstr) {
	char *lineptr;
	char *tokptr;

	printtime();
	for (char *line=strtok_r(commandstr, "\n", &lineptr); 
		 line!=NULL;
		 line=strtok_r(NULL, "\n", &lineptr)) {
		
		char *tok = strtok_r(line, " ", &tokptr);
		if (strcmp(tok, "ping") == 0) {
			printf("ping? pong!\n");
			break;
		} else if (strcmp(tok, "light") == 0) {
			tok = strtok_r(NULL, " ", &tokptr);
			if (strcmp(tok, "on") == 0) {
				relay_on();
				printf(" light on\n");
			} else {
				relay_off();
				printf(" light off\n");
			}
			break;
		} else if (strcmp(tok, "text") == 0) {
			lcd_text(tokptr);
			printf(" set text\n");
			break;
		} else if (strcmp(tok, "time") == 0) {
			tok = strtok_r(NULL, " ", &tokptr);
			if (tok) {
				seconds = atol(tok);
				printf(" set time!\n");
			}
			break;
		} else if (strcmp(tok, "sound") == 0) {
			tok = strtok_r(NULL, " ", &tokptr);
			char *tok2 = strtok_r(NULL, " ", &tokptr);
			if (tok && tok2) {
				int freq = atoi(tok);
				int dur = atoi(tok2);
			
				sound_start(freq, dur);	
			}
			break;
		
		} else {
			printf("Commands:\n");
			printf(" ping - keepalive\n");
			printf(" light <on|off> - turn light on/off\n");
			printf(" time <sec> - set epoche time\n");
			printf(" text <text> - lcd text\n");
			printf(" stats - print stats\n\n");
			break;
		}
	}
}