#include "ExtInterrupts.h"
//Copyright 2015 Palouse RoboSub Club

/*
  This file is part of Sublibinal.

    Sublibinal is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Sublibinal is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Sublibinal.  If not, see <http://www.gnu.org/licenses/>.
 */



void (*callback_int0) (void);
void (*callback_int1) (void);
void (*callback_pcint0) (void);
void (*callback_pcint1) (void);
void (*callback_pcint2) (void);


void initialize_Interrupt(Interrupt_Config config) 
{
    
    //for initialization, we must first set the directionality of the pin and disable any analog values on the pin
    switch (config.extInt) {
    //set the tristate and the PPS
    //disable analog on this pin
        case INT_0:
			
			DDRD &= ~(1<<2); //set PD2 as an input on the tristatei
			//set up the resistor
			if (config.resistor == pullup)
			{
				PORTD |= (1<<2); //enable the pullup resistor	
			} 
			else 
			{
				PORTD &= ~(1<<2); //disable the pullup resistor	 
			}
			
			//configure the polarity
			switch (config.polarity)
			{
				case rising:
					EICRA |= (1<<ISC01) | (1<<ISC00); //set the interrup to trigger on a rising edge
					break;
				case falling:
					EICRA |= (1 << ISC01);
					EICRA &= ~(1<<ISC00);
					break;
				case changing:
					EICRA &= ~(1 << ISC01);
					EICRA |= (1<<ISC00);
					break;
			}
            EIFR &= ~(1<<0);
			EIMSK |= (config.enable << 0); //enable the interrupt?

            callback_int0 = config.callback;//set the callback
            break;
        case INT_1:
            
			DDRD &= ~(1<<3); //set PD2 as an input on the tristatei
			//set up the resistor
			if (config.resistor == pullup)
			{
				PORTD |= (1<<3); //enable the pullup resistor	
			} 
			else 
			{
				PORTD &= ~(1<<3); //disable the pullup resistor	 
			}
			
			//configure the polarity
			switch (config.polarity)
			{
				case rising:
					EICRA |= (1<<ISC11) | (1<<ISC10); //set the interrup to trigger on a rising edge
					break;
				case falling:
					EICRA |= (1 << ISC11);
					EICRA &= ~(1<<ISC10);
					break;
				case changing:
					EICRA &= ~(1 << ISC11);
					EICRA |= (1<<ISC10);
					break;
			}
            EIFR &= ~(1<<1);
			EIMSK |= (config.enable << 1); //enable the interrupt?

            callback_int1 = config.callback;//set the callback
            break;
        case PCINT_0:
			PCICR |= config.enable;
		   //enable all associated pins 
			PCMSK0 = config.pins;	
			callback_pcint0 = config.callback;
            break;
        case PCINT_1:
			PCICR |= config.enable << 1;
			PCMSK1 = config.pins;
			callback_pcint1 = config.callback;
            break;
        case PCINT_2:
			PCICR |= config.enable << 2;
			PCMSK2 = config.pins;
			callback_pcint2 = config.callback;
            break;
        default:
            //error
            break;
    }

}

void disable_Interrupt(Interrupt extInt) {
    //clear the bit
    switch (extInt) {
        case INT_0:
            EIMSK &= ~(1<<0);
            break;
        case INT_1:
            EIMSK &= ~(1<<1);
			break;
        case PCINT_0:
            PCICR &= ~(1<<0);
            break;
        case PCINT_1:
            PCICR &= ~(1<<1);
            break;
        case PCINT_2:
            PCICR &= ~(1<<2);
            break;
        default:
            //error
            break;
    }
}
void enable_Interrupt(Interrupt extInt) {
    //set the bit
	switch (extInt) {
        case INT_0:
            EIMSK |= (1<<0);
            break;
        case INT_1:
            EIMSK |= (1<<1);
			break;
        case PCINT_0:
            PCICR |= (1<<0);
            break;
        case PCINT_1:
            PCICR |= (1<<1);
            break;
        case PCINT_2:
            PCICR |= (1<<2);
            break;
        default:
            //error
            break;
    }
}

ISR(INT1_vect)
{
	if (callback_int1 != NULL)
			callback_int1();
}

ISR(INT0_vect)
{
	if (callback_int0 != NULL)
			callback_int0();
}

ISR(PCINT0_vect)
{
	if (callback_pcint0 != NULL)
			callback_pcint0();
}

ISR(PCINT1_vect)
{
	if (callback_pcint1 != NULL)
			callback_pcint1();
}

ISR(PCINT2_vect)
{
	if (callback_pcint2 != NULL)
			callback_pcint2();
}

