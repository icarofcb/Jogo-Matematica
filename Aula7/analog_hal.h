/*
 *  Title: ADC SERIAL TIMER for general AVR
 *
 *			 dd/mm/yyyy
 *  Created: 10/18/2021 9:22:18 AM
 *  Author: Ícaro Fernando
 *	LinkedIn : https://www.linkedin.com/in/ícaro-fernando-46139919b/
 *  GitHub   : https://github.com/icarofcb
 *
 */ 

#ifndef ANALOG_HAL_H_
#define ANALOG_HAL_H_

#define F_CPU 16000000UL // Clock Speed
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

//===================// Important Libralies //===================//

#include <xc.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>

//===================// Function prototyping //===================//

int16_t analogRead();                     //ADC 
void analogInit();

#endif /* ANALOG_HAL_H_ */