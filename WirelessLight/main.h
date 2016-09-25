/*
 * main.h
 *
 * Created: 12/6/2014 7:08:06 PM
 *  Author: eric
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#define BIT(A) (A<<1)
#define BITS2(A, B) ((A<<1)|(B<<1))
#define BITS3(A, B, C) ((A<<1)|(B<<1)|(C<<1))
#define BITS4(A, B, C, D) (BITS2(A, B) | BITS2(C, D))
#define BITS5(A, B, C, D, E) (BITS2(A, B) | BITS3(C, D, E))

#define CLRBIT(A) (~(A<<1))

#define BAUD 1200

#define F_CPU 16000000
#define MYUBRR ((F_CPU / (BAUD * 16L)) - 1)
#define TX_BUFFER_SIZE 200
#define RX_BUFFER_SIZE 60

#endif /* MAIN_H_ */