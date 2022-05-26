/*
 * Nextion.h
 *
 * Created: 25/05/2022 10:11:53
 *  Author: icaro
 */ 


#ifndef NEXTION_H_
#define NEXTION_H_

#define F_CPU 16000000UL // Clock Speed
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

#include <xc.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void Nextion_sendString(char *ID,char *STR);
void Nextion_sendInt(char *ID,int value);



#endif /* NEXTION_H_ */