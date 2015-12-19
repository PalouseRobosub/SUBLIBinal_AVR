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

Error initialize_Timer(Timer_Config config) {
	Error return_code = ERR_NO_ERR;
	uint16_t period;
	
	switch(config.which_timer)
	{
		case TIMER_0:
		
			//clear arduino configurations
			TCCR0A = 0;
			TIMSK0 = 0;
		
			TCCR0B &= ~((1 << CS02)|(1 << CS01)|(1 << CS00)); //clear the clk prescaler bits
			//figure out the prescaler, set the period register
			if(config.frequency > (float)config.pbclk/((long)255))
			{
				TCCR0B |= (1 << CS00);
				period = config.pbclk/(config.frequency*1);
			}
			else if(config.frequency > (float)config.pbclk/((long)255*8))
			{
				TCCR0B |= (1 << CS01);
				period = config.pbclk/(config.frequency*8);
			}
			else if(config.frequency > (float)config.pbclk/((long)255*64))
			{
				TCCR0B |= (1 << CS01)|(1 << CS00);
				period = config.pbclk/(config.frequency*64);
			}
			else if(config.frequency > (float)config.pbclk/((long)255*256))
			{
				TCCR0B |= (1 << CS02);
				period = config.pbclk/(config.frequency*256);
			}
			else if(config.frequency > (float)config.pbclk/((long)255*1024))
			{
				TCCR0B |= (1 << CS02)|(1 << CS00);
				period = config.pbclk/(config.frequency*1024);
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

			if (config.callback != NULL) {			
				//setup interrupts
				TIMSK0 |= (1 << OCIE0A);
				timer_0_callback = config.callback;
			}
			
			break;
		case TIMER_1:
			//Clear out the configuration
			TCCR1A = 0;
			TCCR0B &= ~((1 << CS12)|(1 << CS11)|(1 << CS10)); //clear the clk prescaler bits
			TIMSK1 = 0;
			
			//figure out the prescaler, set the period register
			if(config.frequency > (float)config.pbclk/((long)65535))
			{
				TCCR1B |= (1 << CS10);
				period = config.pbclk/(config.frequency*1);
			}
			else if(config.frequency > (float)config.pbclk/((long)65535*8))
			{
				TCCR1B |= (1 << CS11);
				period = config.pbclk/(config.frequency*8);
			}
			else if(config.frequency > (float)config.pbclk/((long)65535*64))
			{
				TCCR1B |= (1 << CS11)|(1 << CS10);
				period = config.pbclk/(config.frequency*64);
			}
			else if(config.frequency > (float)config.pbclk/((long)65535*256))
			{
				TCCR1B |= (1 << CS12);
				period = config.pbclk/(config.frequency*256);
			}
			else if(config.frequency > (float)config.pbclk/((long)65535*1024))
			{
				TCCR1B |= (1 << CS12)|(1 << CS10);
				period = config.pbclk/(config.frequency*1024);
			}
			else
			{
				return ERR_BAD_CONFIG;
			}
			OCR1A = period;
			
			//set mode to CTC (WGM bits are 0100)
			TCCR0B |= (1 << WGM12);
			TCCR0B &= ~(1 << WGM13);
			TCCR0A &= ~(1 << WGM11);
			TCCR0A &= ~(1 << WGM10);

			if (config.callback != NULL) {
				//setup interrupts
				TIMSK1 |= (1 << OCIE1A);
				timer_1_callback = config.callback;
			}
			
			break;
		case TIMER_2:
			//clear previous config
			TCCR2A = 0;
			TCCR2B = 0;
			TIMSK2 = 0;
			
			//determine the clock divider and prescaler
			if(config.frequency > (float)config.pbclk/((long)255))
			{
				TCCR2B |= (1 << CS20);
				period = config.pbclk/(config.frequency*1);
			}
			else if(config.frequency > (float)config.pbclk/((long)255*8))
			{
				TCCR2B |= (1 << CS21);
				period = config.pbclk/(config.frequency*8);
			}
			else if(config.frequency > (float)config.pbclk/((long)255*32))
			{
				TCCR2B |= (1 << CS21)|(1 << CS20);
				period = config.pbclk/(config.frequency*32);
			}
			else if(config.frequency > (float)config.pbclk/((long)255*64))
			{
				TCCR2B |= (1 << CS22);
				period = config.pbclk/(config.frequency*64);
			}
			else if(config.frequency > (float)config.pbclk/((long)255*128))
			{
				TCCR2B |= (1 << CS22)|(1 << CS20);
				period = config.pbclk/(config.frequency*128);
			}
			else if(config.frequency > (float)config.pbclk/((long)255*256))
			{
				TCCR2B |= (1 << CS22)|(1 << CS21);
				period = config.pbclk/(config.frequency*256);
			}
			else if(config.frequency > (float)config.pbclk/((long)255*1024))
			{
				TCCR2B |= (1 << CS22)|(1 << CS21)|(1 << CS20);
				period = config.pbclk/(config.frequency*1024);
			}
			else
			{
				return ERR_BAD_CONFIG;
			}
			
			OCR2A = period;
			
			//set CTC mode (WGM bits 010)
			TCCR2B &= ~(1 << WGM22);
			TCCR2A |= (1 << WGM21);
			TCCR2A &= ~(1 << WGM20);
			
			//setup the interrupt mask			
			if (config.callback != NULL) {
				//setup interrupts
				TIMSK2 |= (1 << OCIE2A);
				timer_2_callback = config.callback;
			}
			break;
		default:
			//error?
			return_code = ERR_BAD_CONFIG;
			break;
		
	}
	
	return return_code;
}

Error enable_Timer(Timer_Type which_timer)
{
	switch (which_timer)
	{
		case TIMER_0:
			TCCR0A |= 1<<WGM01;
			break;
		case TIMER_1:
			TCCR1B |= 1<<WGM12;
			break;
		case TIMER_2:
			TCCR2A |= 1<<WGM21;
			break;
	}
}
Error disable_Timer(Timer_Type which_timer)
{
	switch (which_timer)
	{
		case TIMER_0:
			TCCR0A &= ~(1<<WGM00);
			TCCR0A &= ~(1<<WGM01);
			TCCR0B &= ~(1<<WGM02);
			break;
		case TIMER_1:
			TCCR1A &= ~(1<<WGM10);
			TCCR1A &= ~(1<<WGM11);
			TCCR1B &= ~(1<<WGM12);
			TCCR1B &= ~(1<<WGM13);
			break;
		case TIMER_2:
			TCCR2A &= ~(1<<WGM20);
			TCCR2A &= ~(1<<WGM21);
			TCCR2B &= ~(1<<WGM22);
			break;
	}
	
}

ISR(TIMER0_COMPA_vect)
{
	if (timer_0_callback != NULL)
	{
		timer_0_callback();
	}
}

ISR(TIMER1_COMPA_vect)
{
	if (timer_1_callback != NULL)
	{
		timer_1_callback();
	}
}

ISR(TIMER2_COMPA_vect)
{
	if (timer_2_callback != NULL)
	{
		timer_2_callback();
	}
}

