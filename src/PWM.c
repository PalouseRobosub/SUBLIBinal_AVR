#include "PWM.h"
#include "Timer.h"


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


    void initialize_PWM(PWM_Config config) {
    	switch (config.channel)
		{
			case PWM_CH_1:
				//configure the output pin
				//PD5
				DDRD |= 1<<5;

				//enable PWM on the timer in timer configuration
				TCCR0A |= 1<<COM0B1;
				TCCR0A &= ~(1<<COM0B0); // using OCOB
				TCCR0A |= (1 << WGM00) | (1 << WGM01);
				TCCR0B |= 1<<WGM02; //set fast PWM m0de with TOP OCR0A
				OCR0B = config.dutyCycle*OCR0A; //set the duty cycle
				break;
			case PWM_CH_2:
				//Configure Pin PB2
				DDRB |= 1<<2;

				//enable PWM on the timer in configuration
				TCCR1A |= 0b11;
				TCCR1A |= 1<<COM1B1;
				TCCR1A &= ~(1<<COM1B0);
				TCCR1B |= 0b11<<3; //enable PWM fast mode, top at OCR1A
				OCR1B = config.dutyCycle*OCR1A;
				break;
			case PWM_CH_3:
				//Configure PD3
				DDRD |= 1<<3;

				//Configure PWM in the timer configuration
				TCCR2A |= 1<<COM2B1;
				TCCR2A &= ~(1<<COM2B0); //Set mode to PWM fast, non inverted
				TCCR2A |= 0b11;
				TCCR2B |= 1<<WGM22; //PWM mode, fast, top at OCR2A
				OCR2B = config.dutyCycle*OCR2A; //set the duty cycle
				break;
			case PWM_CH_1A_SIMPLE:
				//Configure the pin
				DDRD |= 1<<6;
				if (config.enable == TRUE)
				{
					//Configure the timer for FAST PWM WGM=0b011
					TCCR0A |= (1<<WGM01)|(1<<WGM00);
					TCCR0B &= ~(1<<WGM02);
				}
				
				//Configure COM0A = 0b10 for noninverting output
				TCCR0A |= (1<<COM0A1);
				TCCR0A &= ~(1<<COM0A0);
				
				OCR0A = config.dutyCycle*255; //Set the duty cycle
				break;
			case PWM_CH_1B_SIMPLE:
				//Configure the pin
				DDRD |= 1<<5;
				if (config.enable == TRUE)
				{
					//Configure the timer for FAST PWM WGM=0b011
					TCCR0A |= (1<<WGM01)|(1<<WGM00);
					TCCR0B &= ~(1<<WGM02);
				}
			
				//Configure COM0A = 0b10 for noninverting output
				TCCR0A |= (1<<COM0B1);
				TCCR0A &= ~(1<<COM0B0);
			
				OCR0B = config.dutyCycle*255; //Set the duty cycle
				break;
			case PWM_CH_3A_SIMPLE:
				//Configure the pin
				DDRB |= 1<<3;
				if (config.enable == TRUE)
				{
					//Configure the timer for FAST PWM WGM=0b011
					TCCR2A |= (1<<WGM21)|(1<<WGM20);
					TCCR2B &= ~(1<<WGM22);
				}
			
				//Configure COM0A = 0b10 for noninverting output
				TCCR2A |= (1<<COM2A1);
				TCCR2A &= ~(1<<COM2A0);
			
				OCR2A = config.dutyCycle*255; //Set the duty cycle
				break;
			case PWM_CH_3B_SIMPLE:
				//Configure the pin
				DDRD |= 1<<3;
				if (config.enable == TRUE)
				{
					//Configure the timer for FAST PWM WGM=0b011
					TCCR2A |= (1<<WGM21)|(1<<WGM20);
					TCCR2B &= ~(1<<WGM22);
				}
			
				//Configure COM0A = 0b10 for noninverting output
				TCCR2A |= (1<<COM2B1);
				TCCR2A &= ~(1<<COM2B0);
			
				OCR2B = config.dutyCycle*255; //Set the duty cycle
				break;
		}
	
	}
    
    void enable_PWM(PWM_Channel channel) {
        switch (channel) {
            case PWM_CH_1:
                TCCR0A |= 1<<WGM01;
				TCCR0A |= 1<<WGM00;
				TCCR0B |= 1<<WGM02;
                break;
            case PWM_CH_2:
                TCCR1A |= 1<<WGM10;
				TCCR1A |= 1<<WGM11;
				TCCR1B |= 1<<WGM12;
				TCCR1B |= 1<<WGM13;
                break;
            case PWM_CH_3:
                TCCR2A |= 1<<WGM20;
				TCCR2A |= 1<<WGM21;
				TCCR2B |= 1<<WGM22;
				break;
			case PWM_CH_1A_SIMPLE:
				TCCR0A |= 1<<WGM01;
				TCCR0A |= 1<<WGM00;
				TCCR0B &= ~(1<<WGM02);
				break;
			case PWM_CH_1B_SIMPLE:
				TCCR0A |= 1<<WGM01;
				TCCR0A |= 1<<WGM00;
				TCCR0B &= ~(1<<WGM02);
				break;
			case PWM_CH_3A_SIMPLE:
				TCCR2A |= 1<<WGM21;
				TCCR2A |= 1<<WGM20;
				TCCR2B &= ~(1<<WGM22);
			break;
			case PWM_CH_3B_SIMPLE:
				TCCR2A |= 1<<WGM21;
				TCCR2A |= 1<<WGM20;
				TCCR2B &= ~(1<<WGM22);
			break;
        }
    }
    
    void disable_PWM(PWM_Channel channel) {
         switch (channel) {
            case PWM_CH_1: case PWM_CH_1A_SIMPLE: case PWM_CH_1B_SIMPLE:
                TCCR0A &= ~(1<<WGM01);
				TCCR0A &= ~(1<<WGM00);
				TCCR0B &= ~(1<<WGM02);
                break;
            case PWM_CH_2:
                TCCR1A &= ~(1<<WGM10);
				TCCR1A &= ~(1<<WGM11);
				TCCR1B &= ~(1<<WGM12);
				TCCR1B &= ~(1<<WGM13);
                break;
            case PWM_CH_3: case PWM_CH_3A_SIMPLE: case PWM_CH_3B_SIMPLE:
                TCCR2A &= ~(1<<WGM20);
				TCCR2A &= ~(1<<WGM21);
				TCCR2B &= ~(1<<WGM22);
				break;
        }
    }
    
    void update_PWM(PWM_Channel channel, float dutyCycle) {
        //update the duty cycle of the respective PWM
        switch (channel) {
            case PWM_CH_1:
              	OCR0B = dutyCycle*OCR0A; 
                break;  
			case PWM_CH_1A_SIMPLE:
                OCR0A = dutyCycle*255;
                break;
            case PWM_CH_1B_SIMPLE:
                OCR0B = dutyCycle*255;
                break;
            case PWM_CH_2:
              	OCR1B = dutyCycle*OCR1A; 
            	break;
			case PWM_CH_3:
              	OCR2B = dutyCycle*OCR2A; 
                break;
            case PWM_CH_3A_SIMPLE:
                OCR2A = dutyCycle*255;
                break;
            case PWM_CH_3B_SIMPLE:
                OCR2B = dutyCycle*255;
                break;
                
        }
    }
