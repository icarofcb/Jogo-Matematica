/*
 *  Title: Videogame C/ Nextion
 *
 *			 dd/mm/yyyy
 *  Created: 05/24/2022 9:22:18 AM
 *  Author: Ícaro Fernando
 *	LinkedIn : https://www.linkedin.com/in/ícaro-fernando-46139919b/
 *  GitHub   : https://github.com/icarofcb
 *
 */ 

//===================// Defining Clocks //===================//

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

#include "uart_hal.h"
#include "analog_hal.h"

//===================// Digital Ports Hardware Mapping //===================//

#define D13 PORTB5
#define D12 PORTB4
#define D11 PORTB3
#define D10 PORTB2
#define D9  PORTB1
#define D8  PORTB0

#define D7  PORTD7
#define D6  PORTD6
#define D5  PORTD5
#define D4  PORTD4
#define D3  PORTD3
#define D2  PORTD2
#define D1  PORTD1
#define D0  PORTD0

#define A5 PORTC5
#define A4 PORTC4
#define A3 PORTC3
#define A2 PORTC2
#define A1 PORTC1
#define A0 PORTC0



//===================// Function defines //===================//

#define set_bit(reg,bit)    (reg |= (1<<bit))   // define bit   "DDRx/PORTx,PORTxn"
#define reset_bit(reg,bit)  (reg &= ~(1<<bit))  // reset bit    "DDRx/PORTx,PORTxn"
#define state_bit(reg,bit)  (reg &  (1<<bit))   // compare bit  "PINx,PORTxn"
#define invert_bit(reg,bit) (reg ^= (1<<bit))   // invert o bit "PORTx,PORTxn"

//===================// Function prototyping //===================//
void Nextion_sendString(char *ID,char *STR);
void Nextion_sendInt(char *ID,int value);

//===================// Variable Mapping //===================//

	
const uint16_t T1_init = 0;       // zero for the start point of the timer;
const uint16_t T1_comp = 6250;    // compare value for timer 1 

volatile static int16_t analogvalue = 0;

char msg[50];

char id_t0[] = "t0";

char str_hello[] = "hello";

char endLine[] = "ÿÿÿ";
int val = 45;


	
/* 
 * T1_comp = (time(s) x freq) / prescaler, 
 * for a 12ms interval with a 256 prescaler
 * we have = 0.012 x 16000000 / 256 = 750;
 */ 

//===================// Timer function //===================//
ISR(TIMER1_COMPA_vect)
{
	Nextion_sendInt(id_t0, val);
	
	invert_bit(PORTB,PORTB5);
	
	TCNT1 = T1_init;                      //resets TIMER1	
}

//===================// Main Code //===================//
int main(void)
{

 	    //========== GPIO =========//
 	    set_bit(DDRB,PORTB5);   //PB5 as output led onboard;
 
 	    //========== TIMER1 =========//   
 	    TCCR1A = 0;             //unable PWM, compare mode
		  
 	    //Prescaler 1:256
 	    set_bit(TCCR1B,CS12);
 	    reset_bit(TCCR1B,CS11);
	    reset_bit(TCCR1B,CS10);
 
 	    //Start and compare value for timer1
 	    TCNT1 = T1_init;
 	    OCR1A = T1_comp;
 
 	    //Enable timer 1 interrupt by value compare
 	    TIMSK1 = (1 << OCIE1A);
 
 	    //Global interrupt enable
 	    sei();
 
 	    //========== ADC =========//
		//analogInit();
		
		//========== UART =========//	
		USART_Init(MYUBRR);

	while(1)
	{
		//empty loop.... nice and clean boys
	}
}

void Nextion_sendString(char *ID,char *STR)
{
	char buf[50];
	char aspas[] = "\"";
	char text[] = ".txt=";
	
	strcpy(buf,ID);
	strcat(buf,text);
	strcat(buf,aspas);
	strcat(buf,STR);
	strcat(buf,aspas);
	
	USART_SendString((unsigned char *)buf);
	USART_SendString((unsigned char *)endLine);
	
}

void Nextion_sendInt(char *ID,int value)
{
	char buf[50];
	sprintf(buf,"%d",value);
	Nextion_sendString(ID,buf);
}