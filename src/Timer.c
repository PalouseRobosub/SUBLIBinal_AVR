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

volatile char cs_0 = 0, cs_1 = 0, cs_2 = 0;

Error initialize_Timer(Timer_Config config) {
	Error return_code = ERR_NO_ERR;
	uint16_t period;
	
	double error_1, error_8, error_64, error_256, error_1024, error_32, error_128;
	if (config.precision == PRECISE) 
	{
		switch(config.which_timer)
		{
			case TIMER_0:
		
				//clear arduino configurations
				TCCR0A = 0;
				TIMSK0 = 0;
				TCCR0B = 0;
				TCNT0 = 0;

				cs_0 &= ~((1 << CS02)|(1 << CS01)|(1 << CS00)); //clear the clk prescaler bits
				//figure out the prescaler, set the period register
				if(config.frequency > (float)config.pbclk/((long)255))
				{
					cs_0 |= (1 << CS00);
					period = config.pbclk/(config.frequency*1);
				}
				else if(config.frequency > (float)config.pbclk/((long)255*8))
				{
					cs_0 |= (1 << CS01);
					period = config.pbclk/(config.frequency*8);
				}
				else if(config.frequency > (float)config.pbclk/((long)255*64))
				{
					cs_0 |= (1 << CS01)|(1 << CS00);
					period = config.pbclk/(config.frequency*64);
				}
				else if(config.frequency > (float)config.pbclk/((long)255*256))
				{
					cs_0 |= (1 << CS02);
					period = config.pbclk/(config.frequency*256);
				}
				else if(config.frequency > (float)config.pbclk/((long)255*1024))
				{
					cs_0 |= (1 << CS02)|(1 << CS00);
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

				if (config.enabled == TRUE)
				{
					TCCR0B |= cs_0;
				}

				if (config.callback != NULL) {			
					//setup interrupts
					TIMSK0 |= (1 << OCIE0A);
					timer_0_callback = config.callback;
				}
			
				break;
			case TIMER_1:
				//Clear out the configuration
				TCCR1A = 0;
				TCCR1B = 0;
				TCNT1 = 0;
				cs_1 = 0; //clear the clk prescaler bits
				TIMSK1 = 0;
					
				//figure out the prescaler, set the period register
				if(config.frequency > (float)config.pbclk/((long)65535))
				{
					cs_1 |= (1 << CS10);
					period = config.pbclk/(config.frequency*1);
				}
				else if(config.frequency > (float)config.pbclk/((long)65535*8))
				{
					cs_1 |= (1 << CS11);
					period = config.pbclk/(config.frequency*8);
				}
				else if(config.frequency > (float)config.pbclk/((long)65535*64))
				{
					cs_1 |= (1 << CS11)|(1 << CS10);
					period = config.pbclk/(config.frequency*64);
				}
				else if(config.frequency > (float)config.pbclk/((long)65535*256))
				{
					cs_1 |= (1 << CS12);
					period = config.pbclk/(config.frequency*256);
				}
				else if(config.frequency > (float)config.pbclk/((long)65535*1024))
				{
					cs_1 |= (1 << CS12)|(1 << CS10);
					period = config.pbclk/(config.frequency*1024);
				}
				else
				{
					return ERR_BAD_CONFIG;
				}
				OCR1A = period;
			
				//set mode to CTC (WGM bits are 0100)
				TCCR1B |= (1 << WGM12);
				TCCR1B &= ~(1 << WGM13);
				TCCR1A &= ~(1 << WGM11);
				TCCR1A &= ~(1 << WGM10);

				TCCR1B &= ~((1<<CS10)|(1<<CS11)|(1<<CS12));
				if (config.enabled == TRUE)
				{
					TCCR1B|= cs_1;
				}

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
				cs_2 = 0;
				TCNT2 = 0;

				//determine the clock divider and prescaler
				if(config.frequency > (float)config.pbclk/((long)255))
				{
					cs_2 |= (1 << CS20);
					period = config.pbclk/(config.frequency*1);
				}
				else if(config.frequency > (float)config.pbclk/((long)255*8))
				{
					cs_2 |= (1 << CS21);
					period = config.pbclk/(config.frequency*8);
				}
				else if(config.frequency > (float)config.pbclk/((long)255*32))
				{
					cs_2 |= (1 << CS21)|(1 << CS20);
					period = config.pbclk/(config.frequency*32);
				}
				else if(config.frequency > (float)config.pbclk/((long)255*64))
				{
					cs_2 |= (1 << CS22);
					period = config.pbclk/(config.frequency*64);
				}
				else if(config.frequency > (float)config.pbclk/((long)255*128))
				{
					cs_2 |= (1 << CS22)|(1 << CS20);
					period = config.pbclk/(config.frequency*128);
				}
				else if(config.frequency > (float)config.pbclk/((long)255*256))
				{
					cs_2 |= (1 << CS22)|(1 << CS21);
					period = config.pbclk/(config.frequency*256);
				}
				else if(config.frequency > (float)config.pbclk/((long)255*1024))
				{
					cs_2 |= (1 << CS22)|(1 << CS21)|(1 << CS20);
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
			
				if (config.enabled == TRUE)
					TCCR2B |= cs_2;

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
	}
	else //imprecise mode
	{
		switch (config.which_timer) {
			case TIMER_0:
				//Clear previous config
				TCCR0A = 0;
				TIMSK0 = 0;
				TCCR0B = 0;
				TCNT0 = 0;
				cs_0 = 0;


				//Find the best divider
				error_1 = config.pbclk/((double)255*1) - config.frequency;
				if (error_1 < 0)
					error_1 *= -1;
				error_8 = config.pbclk/((double)255*8) - config.frequency;
				if (error_8 < 0)
					error_8 *= -1;
				error_64 = config.pbclk/((double)255*64) - config.frequency;
				if (error_64 < 0)
					error_64 *= -1;
				error_256 = config.pbclk/((double)255*256) - config.frequency;
				if (error_256 < 0)
					error_256 *= -1;
				error_1024 = config.pbclk/((double)255*1024) - config.frequency;
				if (error_1024 < 0)
					error_1024 *= -1;
				
				if (error_1 < error_8)
					//div is 1
					cs_0 |= 1<<CS00;
				else if (error_8 < error_64)
					//Div is 8
					cs_0 |= 1<<CS01;
				else if (error_64 < error_256)
					//Div is 64
					cs_0 |= (1<<CS01)|(1<<CS00);
				else if (error_256 < error_1024)
					//Div is 256
					cs_0 |= 1<<CS02;
				else
					//Div is 1024
					cs_0 |= (1<<CS02)|(1<<CS00);
				
				if (config.enabled == TRUE)
					TCCR0B |= cs_0;
			
				//Set Mode
				//WGM = 0b000

				//Set interrupts
				//setup the interrupt mask
				if (config.callback != NULL) {
					//setup interrupts
					TIMSK0 |= (1 << TOIE0);
					timer_0_callback = config.callback;
				}
				break;
			case TIMER_1:
				//Clear previous config
				TCCR1A = 0;
				TIMSK1 = 0;
				TCCR1B = 0;
				TCNT1 = 0;
				cs_1 = 0;

				//Find the best divider
				error_1 = config.pbclk/((double)65535*1) - config.frequency;
				if (error_1 < 0)
					error_1 *= -1;

				error_8 = config.pbclk/((double)65535*8) - config.frequency;
				if (error_8 < 0)
					error_8 *= -1;

				error_64 = config.pbclk/((double)65535*64) - config.frequency;
				if (error_64 < 0)
					error_64 *= -1;

				error_256 = config.pbclk/((double)65535*256) - config.frequency;
				if (error_256 < 0)
					error_256 *= -1;

				error_1024 = config.pbclk/((double)65535*1024) - config.frequency;
				if (error_1024 < 0)
					error_1024 *= -1;

				if (error_1 < error_8)
					//div is 1
					cs_1 |= 1<<CS10;
				else if (error_8 < error_64)
					//Div is 8
					cs_1 |= 1<<CS11;
				else if (error_64 < error_256)
					//Div is 64
					cs_1 |= (1<<CS11)|(1<<CS10);
				else if (error_256 < error_1024)
					//Div is 256
					cs_1 |= 1<<CS12;
				else
					//Div is 1024
					cs_1 |= (1<<CS12)|(1<<CS10);
								
				if (config.enabled == TRUE)
					TCCR1B |= cs_1;
				
				//Set Mode
				//WGM = 0b000 already done by clearing registers
				//Set interrupts
				//setup the interrupt mask
				if (config.callback != NULL) {
					//setup interrupts
					TIMSK1 |= (1 << TOIE1);
					timer_1_callback = config.callback;
				}
				break;
			case TIMER_2:
				//Clear previous config
				TCCR2A = 0;
				TIMSK2 = 0;
				TCCR2B = 0;
				TCNT2 = 0;
				cs_2 = 0;

				//Find the best divider
				error_1 = config.pbclk/((double)255*1) - config.frequency;
				if (error_1 < 0)
					error_1 *= -1;
				error_8 = config.pbclk/((double)255*8) - config.frequency;
				if (error_8 < 0)
					error_8 *= -1;
				error_32 = config.pbclk/((double)255*32) - config.frequency;
				if (error_32 < 0)
					error_32 *= -1;
				error_64 = config.pbclk/((double)255*64) - config.frequency;
				if(error_64 < 0)
					error_64 *= -1;
				error_128 = config.pbclk/((double)255*128) - config.frequency;
				if(error_128 < 0)
					error_128 *= -1;
				error_256 = config.pbclk/((double)255*256) - config.frequency;
				if (error_256 < 0)
					error_256 *= -1;
				error_1024 = config.pbclk/((double)255*1024) - config.frequency;
				if (error_1024 < 0)
					error_1024 *= -1;
				
				if (error_1 < error_8)
					//div is 1
					cs_2 |= 1<<CS20;
				else if (error_8 < error_32)
					//Div is 8
					cs_2 |= 1<<CS21;
				else if (error_32 < error_64)
					//Div is 32
					cs_2 |= (1<<CS21)|(1<<CS20);
				else if (error_64 < error_128)
					//Div is 64
					cs_2 |= 1<<CS22;
				else if (error_128 < error_256)
					//Div 128
					cs_2 |= (1<<CS22)|(1<<CS20);
				else if (error_256 < error_1024)
					//Div 256
					cs_2 |= (1<<CS22)|(1<<CS21);
				else
					//Div 1024
					cs_2 |= (1<<CS22)|(1<<CS21)|(1<<CS20);

				if (config.enabled == TRUE)
					TCCR2B |= cs_2;


				//Set Mode
				//WGM = 0b000

				//Set interrupts
				//setup the interrupt mask
				if (config.callback != NULL) {
					//setup interrupts
					TIMSK2 |= (1 << TOIE2);
					timer_2_callback = config.callback;
				}
				break;
			default:
				return_code = ERR_BAD_CONFIG;
				break;
			
		}
	}
	
	return return_code;
}

Error enable_Timer(Timer_Type which_timer)
{
	switch (which_timer)
	{
		case TIMER_0:
			TCCR0B |= cs_0;
			break;
		case TIMER_1:
			TCCR1B |= cs_1;
			break;
		case TIMER_2:
			TCCR2B |= cs_2;
			break;
		default: return ERR_INVALID_CHANNEL;
	}
	
	return ERR_NO_ERR;
}
Error disable_Timer(Timer_Type which_timer)
{
	switch (which_timer)
	{
		case TIMER_0:
			TCCR0B &= ~((1<<CS00)|(1<<CS01)|(1<<CS02));
			break;
		case TIMER_1:
			TCCR1B &= ~((1<<CS10)|(1<<CS11)|(1<<CS12));
			break;
		case TIMER_2:
			TCCR2B &= ~((1<<CS20)|(1<<CS21)|(1<<CS22));
			break;
		default: return ERR_INVALID_CHANNEL;
	}
	
	return ERR_NO_ERR;
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

ISR(TIMER0_OVF_vect)
{
	if (timer_0_callback != NULL)
	{
		timer_0_callback();
	}
}

ISR(TIMER1_OVF_vect)
{
	if (timer_1_callback != NULL)
	{
		timer_1_callback();
	}
}

ISR(TIMER2_OVF_vect)
{
	if (timer_2_callback != NULL)
	{
		timer_2_callback();
	}
}

