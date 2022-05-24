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

#include "analog_hal.h"

void analogInit()
{
	ADCSRA  = 0x93;                //initiates the ADC with 8 dividing base
	ADMUX   = 0x40;                //voltage reference internal 5V
}

int16_t analogRead(int16_t channel)
{
	static int analogH, analogL, analog;    //local variables for ADC
	
	ADMUX   = 0x40 + channel;               //voltage reference internal 5V and ADC channel from function calling

	ADCSRA |= (1<<ADSC);                    //starts the ADC conversion
	
	while(!(ADCSRA&=~(1<<ADIF)));           //wait for ADC to finish
	
	ADCSRA |= (1<<ADIF);                    //clear ADIF with a transition from LOW to HIGH (ADIF is a bit for "end of conversion")
	
	analogL = ADCL;                         //LSB 
	analogH = ADCH;                         //MSB
											//it's a 10 bits ADC converter, then you need two registers to store your value
	
	analog  = (analogH<<8) | analogL;       //bit shift and sum to get your MSB and LSB together

	ADMUX   = 0x40;							//channel reset
	return analog;                          //return your ADC value
	
} //end ad_conv