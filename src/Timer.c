/*
 * Timer.c
 *
 * Created: 11/25/2015 3:06:47 PM
 *  Author: james
 */ 

#include "Timer.h"

void (*timer_0_callback) (void);
void (*timer_1_callback) (void);
void (*timer_2_callback) (void);

Error initialize_timer(Timer_Config config) {
	Error return_code = ERR_NO_ERR;
	uint16_t period;
	
	switch(config.which_timer)
	{
		case TIMER_0:
			TCCR0B &= ~((1 << CS02)|(1 << CS01)|(1 << CS00)); //clear the clk prescaler bits
			//figure out the prescaler, set the period register
			if(config.frequency > (float)config.pbclk/(UINT8_MAX))
			{
				TCCR0B |= (1 << CS00);
				period = config.pbclk/(config.frequency*1);
			}
			else if(config.frequency > (float)config.pbclk/(UINT8_MAX*8))
			{
				TCCR0B |= (1 << CS01);
				period = config.pbclk/(config.frequency*8);
			}
			else if(config.frequency > (float)config.pbclk/(UINT8_MAX*64))
			{
				TCCR0B |= (1 << CS01)|(1 << CS00);
				period = config.pbclk/(config.frequency*64);
			}
			else if(config.frequency > (float)config.pbclk/(UINT8_MAX*256))
			{
				TCCR0B |= (1 << CS02);
				period = config.pbclk/(config.frequency*256);
			}
			else if(config.frequency > (float)config.pbclk/(UINT8_MAX*1024))
			{
				TCCR0B |= (1 << CS02)|(1 << CS00);
				period = config.pbclk/(config.frequency*1);
			}
			else
			{
				return ERR_BAD_CONFIG;
			}
			OCR0A = period;
			
			//set mode to CTC (WGM bits are 010)
			TCCR0B &= ~(1 << WGM02);
			TCCR0A |= (1 << WGM01);
			TCCR0A &= ~(1 << WGM00);
			
			//setup interrupts
			TIMSK0 = (1 << OCIE0A);
			
			break;
		case TIMER_1:
			return ERR_BAD_CONFIG;
			break;
		case TIMER_2:
			return ERR_BAD_CONFIG;
			break;
		default:
			//error?
			return_code = ERR_BAD_CONFIG;
			break;
		
	}
	
	return return_code;
}

ISR(TIMER0_COMPA_vect)
{
	if (timer_1_callback != NULL) 
	{
		timer_1_callback();
	}

	TIFR0 |= (1 << OCF0A); //clear the interrupt flag
}