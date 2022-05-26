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
#include "Nextion.h"

//===================// Digital Ports Hardware Mapping //===================//

#define D13 PORTB5

#define row_D  PORTB1 //D9
#define row_C  PORTB0 //D8

#define row_B  PORTD7 //D7
#define row_A  PORTD6 //D6
#define col_4  PORTD5 //D5
#define col_3  PORTD4 //D4
#define col_2  PORTD3 //D3
#define col_1  PORTD2 //D2

//===================// Function defines //===================//

#define set_bit(reg,bit)    (reg |= (1<<bit))   // define bit   "DDRx/PORTx,PORTxn"
#define reset_bit(reg,bit)  (reg &= ~(1<<bit))  // reset bit    "DDRx/PORTx,PORTxn"
#define state_bit(reg,bit)  (reg &  (1<<bit))   // compare bit  "PINx,PORTxn"
#define invert_bit(reg,bit) (reg ^= (1<<bit))   // invert o bit "PORTx,PORTxn"

//===================// Function prototyping //===================//
void readKeyboard();
void store(char value);
void startGame();
void waitPlayers();
void gameRun();
void clearNumbers(char player);
void menu();

//===================// Variable Mapping //===================//
const uint16_t T1_init = 0;       // zero for the start point of the timer;
const uint16_t T1_comp = 750;    // compare value for timer 1 

volatile static int16_t analogvalue = 0;

char idTituloB[] = "t6";
char idTituloC[] = "t3";

char idSenhaB[]    = "t7"; //B 14
char idRespostaB[] = "t5";

char idSenhaC[]    = "t2"; //C 15
char idRespostaC[] = "t4";

char idCompB[] = "t9";
char idCompC[] = "t10";

volatile char algarismo  = 0x00;                  //variável para armazenar o número pressionado no teclado
volatile char numero     = 0x00;

uint8_t senhaB = 32,
		senhaC = 46,
		RespB  = 0,
		RespC  = 0;

uint8_t gameMode = 0x00,
		player = 0;

//===================// Timer function //===================//
ISR(TIMER1_COMPA_vect)
{
	readKeyboard();
	
	invert_bit(PORTB,D13);
	
	TCNT1 = T1_init;                      //resets TIMER1	
}

//===================// Main Code //===================//
int main(void)
{

 	    //========== GPIO =========//
 	    set_bit(DDRB,D13);   
		set_bit(DDRD,col_1);
		set_bit(DDRD,col_2);
		set_bit(DDRD,col_3);
		set_bit(DDRD,col_4);
		
		reset_bit(DDRD,row_A);
		reset_bit(DDRD,row_B);
		reset_bit(DDRB,row_C);
		reset_bit(DDRB,row_D);
		
		PORTD = 0;
		PORTB = 0;
		
		set_bit(PORTD,col_1);
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
		
		
		menu();
		
		
	while(1)
	{
		if(gameMode==0x00)
		{
			if(algarismo==0x00)
			{
				startGame();
			
			}else if((algarismo!=0)){
		
				waitPlayers();	
			}
			
		}else gameRun();
	}
}


void readKeyboard()
{
	static char control  = 0x01;                  //variável de controle de teclado
	static char rowA_flag1 = 0x00,
				rowB_flag1 = 0x00,
				rowC_flag1 = 0x00,
				rowD_flag1 = 0x00;
				
	static char rowA_flag2 = 0x00,
				rowB_flag2 = 0x00,
				rowC_flag2 = 0x00,
				rowD_flag2 = 0x00;
	
	static char rowA_flag3 = 0x00,
				rowB_flag3 = 0x00,
				rowC_flag3 = 0x00,
				rowD_flag3 = 0x00;

	static char rowA_flag4 = 0x00,
				rowB_flag4 = 0x00,
				rowC_flag4 = 0x00,
				rowD_flag4 = 0x00;
	
	if(state_bit(PIND,col_1) && control == 0x01)
	{
		control = 0x02;
		reset_bit(PORTD,col_1);
		set_bit(PORTD,col_2);
		set_bit(PORTD,col_3);
		set_bit(PORTD,col_4);
		
		if(!(state_bit(PIND,row_A)) && !rowA_flag1)	  
		{
			rowA_flag1 = 1;
			
		}else if(state_bit(PIND,row_A) && rowA_flag1)
		{
			rowA_flag1 = 0;
			store(1);
			_delay_ms(50);
		}else if(!(state_bit(PIND,row_B)) && !rowB_flag1)
		{
			rowB_flag1 = 1;
			
		}else if(state_bit(PIND,row_B) && rowB_flag1)
		{
			rowB_flag1 = 0;
			store(4);
			_delay_ms(50);
		}else if(!(state_bit(PINB,row_C)) && !rowC_flag1)
		{
			rowC_flag1 = 1;
			
		}if(state_bit(PINB,row_C) && rowC_flag1)
		{
			rowC_flag1 = 0;
			store(7);
			_delay_ms(50);
		}else if(!(state_bit(PINB,row_D)) && !rowD_flag1)
		{
			rowD_flag1 = 1;
			
		}else if(state_bit(PINB,row_D) && rowD_flag1)
		{
			rowD_flag1 = 0;
			store(11);
			_delay_ms(50);
		}
		
	}else if(state_bit(PIND,col_2) && control == 0x02)
	{
		control = 0x03;
		set_bit(PORTD,col_1);
		reset_bit(PORTD,col_2);
		set_bit(PORTD,col_3);
		set_bit(PORTD,col_4);
		
				if(!(state_bit(PIND,row_A)) && !rowA_flag2)
				{
					rowA_flag2 = 1;
					
				}else if(state_bit(PIND,row_A) && rowA_flag2)
				{
					rowA_flag2 = 0;
					store(2);
					_delay_ms(50);
				}else if(!(state_bit(PIND,row_B)) && !rowB_flag2)
				{
					rowB_flag2 = 1;
					
				}else if(state_bit(PIND,row_B) && rowB_flag2)
				{
					rowB_flag2 = 0;
					store(5);
					_delay_ms(50);
				}else if(!(state_bit(PINB,row_C)) && !rowC_flag2)
				{
					rowC_flag2 = 1;
					
				}if(state_bit(PINB,row_C) && rowC_flag2)
				{
					rowC_flag2 = 0;
					store(8);
					_delay_ms(50);
				}else if(!(state_bit(PINB,row_D)) && !rowD_flag2)
				{
					rowD_flag2 = 1;
					
				}else if(state_bit(PINB,row_D) && rowD_flag2)
				{
					rowD_flag2 = 0;
					store(0);
					_delay_ms(50);
				}
				
	}else if(state_bit(PIND,col_3) && control == 0x03)
	{
		control = 0x04;
		set_bit(PORTD,col_1);
		set_bit(PORTD,col_2);
		reset_bit(PORTD,col_3);
		set_bit(PORTD,col_4);

				if(!(state_bit(PIND,row_A)) && !rowA_flag3)
				{
					rowA_flag3 = 1;
					
				}else if(state_bit(PIND,row_A) && rowA_flag3)
				{
					rowA_flag3 = 0;
					store(3);
					_delay_ms(50);
				}else if(!(state_bit(PIND,row_B)) && !rowB_flag3)
				{
					rowB_flag3 = 1;
					
				}else if(state_bit(PIND,row_B) && rowB_flag3)
				{
					rowB_flag3 = 0;
					store(6);
					_delay_ms(50);
				}else if(!(state_bit(PINB,row_C)) && !rowC_flag3)
				{
					rowC_flag3 = 1;
					
				}if(state_bit(PINB,row_C) && rowC_flag3)
				{
					rowC_flag3 = 0;
					store(9);
					_delay_ms(50);
				}else if(!(state_bit(PINB,row_D)) && !rowD_flag3)
				{
					rowD_flag3 = 1;
					
				}else if(state_bit(PINB,row_D) && rowD_flag3)
				{
					rowD_flag3 = 0;
					store(12);
					_delay_ms(50);
				}
		
	}else if(state_bit(PIND,col_4) && control == 0x04)
	{
		control = 0x01;
		set_bit(PORTD,col_1);
		set_bit(PORTD,col_2);
		set_bit(PORTD,col_3);
		reset_bit(PORTD,col_4);
	
				if(!(state_bit(PIND,row_A)) && !rowA_flag4)
				{
					rowA_flag4 = 1;
					
				}else if(state_bit(PIND,row_A) && rowA_flag4)
				{
					rowA_flag4 = 0;
					store(17);
					_delay_ms(50);
				}else if(!(state_bit(PIND,row_B)) && !rowB_flag4)
				{
					rowB_flag4 = 1;
					
				}else if(state_bit(PIND,row_B) && rowB_flag4)
				{
					rowB_flag4 = 0;
					store(14);
					_delay_ms(50);
				}else if(!(state_bit(PINB,row_C)) && !rowC_flag4)
				{
					rowC_flag4 = 1;
					
				}if(state_bit(PINB,row_C) && rowC_flag4)
				{
					rowC_flag4 = 0;
					store(15);
					_delay_ms(50);
					
				}else if(!(state_bit(PINB,row_D)) && !rowD_flag4)
				{
					rowD_flag4 = 1;
					
				}else if(state_bit(PINB,row_D) && rowD_flag4)
				{
					rowD_flag4 = 0;
					store(16);
					_delay_ms(50);
				}
	}
	
}

void store(char value)                   
{   
	TIMSK1 = 0;                          
	static char digito  = 0;
	
	algarismo = value;
	
	if(!digito)
	{
		if(value<10) numero = (value*10);
		digito = 1;
		
	}else{
		if(value<10) numero += algarismo;
		digito  = 0;
	}
	
	if(gameMode!=0x00)
	{
		if(!player)
		{
			Nextion_sendInt(idRespostaB,numero);	
		}else Nextion_sendInt(idRespostaC,numero);
	}
	
	_delay_ms(250);
	                       
	
	TIMSK1 = (1 << OCIE1A);
	
}

void menu()
{
	char msg1[] = "Senha B";
	char msg2[] = "Senha C";
	char msg3[] = "t7.pw=1";
	char msg4[] = "t2.pw=1";
	char endLine1[] = "ÿÿÿ";
	char clear[] = " ";
	
	Nextion_sendString(idCompB,clear);
	Nextion_sendString(idCompC,clear);
	
	Nextion_sendString(idTituloB, msg1);
	Nextion_sendString(idTituloC, msg2);
	
	Nextion_sendString(idRespostaB,clear);
	Nextion_sendString(idRespostaB,clear);
	
	USART_SendString((unsigned char *)msg3);
	USART_SendString((unsigned char *)endLine1);
	
	USART_SendString((unsigned char *)msg4);
	USART_SendString((unsigned char *)endLine1);
	
	gameMode  = 0;
	numero    = 0;
	algarismo = 0;
		
}
void startGame()
{
	char msg1[]  = "Pressione tecla B";
	char msg2[]  = "Pressione tecla C";
	char clear[] = " ";
	
	Nextion_sendString(idRespostaB,msg1);
	Nextion_sendString(idRespostaC,msg2);
	_delay_ms(300);
	Nextion_sendString(idRespostaB,clear);
	Nextion_sendString(idRespostaC,clear);
	_delay_ms(300);
	
}

void waitPlayers()
{
	char msg1[] = "Player B pronto";
	char msg2[] = "Player C pronto";
	
	static uint8_t pB_ready = 0,
				   pC_ready = 0;

	if(algarismo==14 && !pB_ready)
	{
		pB_ready = 1;
		Nextion_sendString(idRespostaB,msg1);
	}
	if(algarismo==15 && !pC_ready)
	{
		pC_ready = 1;
		Nextion_sendString(idRespostaC,msg2);
	}
		
	if(pB_ready && pC_ready)
	{
		Nextion_sendInt(idSenhaB,senhaB);
		Nextion_sendInt(idSenhaC,senhaC);
		
		gameMode = 0x01;
	}

	_delay_ms(50);
}

void gameRun()
{
	static uint8_t ctrl   = 0;
				   
	char menor[] = "<",
		 maior[] = ">",
		 igual[] = "=";
				   
	
	if(!ctrl)
	{
		clearNumbers(player);
		ctrl = 1;	
	}
	
	
	
	if(!player)//B
	{
		if(algarismo == 16)
		{	
			TIMSK1 = 0; 
			_delay_ms(50);
			//clearNumbers(player);
			
			if(numero<senhaB)       {Nextion_sendString(idCompB,menor); player = 1; clearNumbers(player);}
			else if(numero>senhaB)  {Nextion_sendString(idCompB,maior); player = 1; clearNumbers(player);} 
			else if(numero==senhaB) {Nextion_sendString(idCompB,igual); player = 1; clearNumbers(player); _delay_ms(10000); menu();} // Tirar clear number e colcoar sequencia de vitoria
			_delay_ms(50);
			TIMSK1 = (1 << OCIE1A);
		}
		//Nextion_sendInt(idRespostaB,numero);
			
	}else{ //C
		
		if(algarismo == 16)
		{
			TIMSK1 = 0;
			_delay_ms(50);
			//clearNumbers(player);
			
			if(numero<senhaC)       {Nextion_sendString(idCompC,menor); player = 0; clearNumbers(player);}
			else if(numero>senhaC)  {Nextion_sendString(idCompC,maior); player = 0; clearNumbers(player);}
			else if(numero==senhaC) {Nextion_sendString(idCompC,igual); player = 0; clearNumbers(player); _delay_ms(10000); menu();} // Tirar clear number e colcoar sequencia de vitoria
			
			_delay_ms(50);
			TIMSK1 = (1 << OCIE1A);
		}
		//Nextion_sendInt(idRespostaC,numero);
	}	
	
	if(algarismo == 11)
	{
		clearNumbers(player);
	}
	
	if(algarismo == 12)
	{
		menu();
	}
	
	_delay_ms(100);
}

void clearNumbers(char player)
{
	char nada[] = "0";
	numero    = 0;
	algarismo = 0;
	
	if(!player)
	{
		Nextion_sendString(idRespostaB,nada);
	}else Nextion_sendString(idRespostaC,nada);
	
}