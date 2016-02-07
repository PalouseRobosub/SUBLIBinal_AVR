
#include "ADC.h"

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


//callback functions
void (*ADC_callback) (void);

//channel data structures
ADC_Data adc_data;
static ADC_Node currentNode_Global;


ADC_Data* initialize_ADC(ADC_Config config) {

    //setup the rx and tx buffers
    adc_data.Results_queue = create_queue(config.result_buffer_ptr, config.result_buffer_size);
    adc_data.Work_queue = create_queue(config.work_buffer_ptr, config.work_buffer_size);

    ADC_callback = config.callback; //link the callback function

    adc_data.is_idle = TRUE; //set the ADC module to idling

    //Select the analog inputs to the ADC multiplexers in AD1CHS
	ADMUX &= ~(1<<REFS1|1<<REFS0|1<<ADLAR) ; //use AREF pin, right adjusted

    //set up pins for analog function
    setup_ADC_pins(config.channels);
   
	ADCSRA = 1<<ADEN |(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)| 1 << ADIE; //turn on the ADC and enable the interrupt -> use 128 divider
	ADCSRB = 0;

    return &adc_data;
}

int read_ADC(ADC_Node node) {
    int status;
    

    //if the bus is idling, force-start it
    if (adc_data.is_idle) 
	{	
		currentNode_Global = node;
        adc_data.is_idle = FALSE; //flag that the bus is working now
		ADMUX = (ADMUX & ~(0b1111)) | currentNode_Global.channel; //set the AD MUX to the input channel of this sample
		ADCSRA |= 1<<ADSC; //force start the ADC
	}
	else
    	status = enqueue(&(adc_data.Work_queue), (uint8*) & node, sizeof (node));


    return status;
}

void bg_process_ADC(void) {
    static ADC_Node current_node; //static to improve speed

    while(!dequeue(&(adc_data.Results_queue), (uint8*) & current_node, sizeof(current_node)))
    {
        if(current_node.callback != NULL){
            current_node.callback(current_node);
        }

    }
}

ISR(ADC_vect) {
	//Debug code
	DDRB |= 1<<4;


	if (currentNode_Global.device_id == 0x03)
		PORTB |= 1<<4;
	else if (currentNode_Global.device_id == 0x04)
		PORTB &= ~(1<<4);

    currentNode_Global.data = ADC;

	enqueue(&(adc_data.Results_queue), (uint8*) &currentNode_Global, sizeof(currentNode_Global));

    if (dequeue(&(adc_data.Work_queue), (uint8*) &currentNode_Global, sizeof(currentNode_Global))) //load next node from the queue
    {
        adc_data.is_idle = TRUE; //flag that the bus is idle (nothing in the send queue)
    } else {
        adc_data.is_idle = FALSE; //flag that the bus is working now
		ADMUX = (ADMUX & ~(0b1111)) | currentNode_Global.channel; //set the AD MUX to the input channel of this sample
        ADCSRA |= 1<<ADSC; //force start the ADC
    }

    if (ADC_callback != NULL) {
        ADC_callback();
    }
}

//setup the proper pins for analog use
void setup_ADC_pins(uint16 channels) {
	
	//configure the pins as inputs
	DDRC &= ~(channels);
	//disable the digital buffer
	DIDR0 = channels;
}
