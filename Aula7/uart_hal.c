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

#include "uart_hal.h"

volatile uint8_t uart_tx_done = 1;  //1 as default	

ISR(USART_TX_vect)
{
	uart_tx_done = 1;
}

void USART_Init(unsigned int ubrr)
{
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/*Enable receiver and transmitter */
	UCSR0B = (0<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop bit */
	//UCSR0C = (1<<USBS0)|(3<<UCSZ00);
	UCSR0C = 0x06;
}

void USART_Transmit(unsigned char data)
{
	uart_tx_done = 0;
	
	/* Wait for empty transmit buffer */
	while (!(UCSR0A & (1<<UDRE0)))
	;
	/* Put data into buffer, sends the data */
	if(uart_tx_done!=1)
	{
		UDR0 = data;
	}
}

void USART_SendString(unsigned char *data)
{
	uint16_t i = 0;
	while(data[i] != 0)
	{
		USART_Transmit(data[i]);
		i++;	
	}
	//USART_Transmit(data[i]);
}

void USART_SendInt(int16_t value)
{
	char buf[50];
	sprintf(buf,"%d",value);
	USART_SendString((uint8_t *)buf);
}
