/*
 * Nextion.c
 *
 * Created: 25/05/2022 10:12:22
 *  Author: icaro
 */ 

#include "Nextion.h"
#include "uart_hal.h"

char endLine[] = "ÿÿÿ";

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
