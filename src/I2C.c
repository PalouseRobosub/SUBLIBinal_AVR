/*
 * I2C.c
 *
 * Created: 11/25/2015 3:05:30 PM
 *  Author: james
 */ 

#include "I2C.h"

//callback functions
void (*I2C_1_callback) (void);

I2C_Data i2c_data;
	
Error initialize_I2C(I2C_Config config) 
{
	Error status = ERR_NO_ERR;
	
	TWBR = ((config.pb_clk/I2C_SPEED) - 16)/2; //calculate the proper divider

	//setup the rx and tx buffers
	i2c_data.rx_queue = create_queue(config.rx_buffer_ptr, config.rx_buffer_size);
	i2c_data.tx_queue = create_queue(config.tx_buffer_ptr, config.tx_buffer_size);

	I2C_1_callback = config.callback; //link the callback function

	i2c_data.tx_is_idle = TRUE; //set the I2C state machine to idling

	TWCR = (1 << TWEN) | (1 << TWIE);

	return status;
}

Error send_I2C(I2C_Node node)
{
	Error status = ERR_NO_ERR;
	
	status = enqueue(&(i2c_data.tx_queue), (uint8_t*) & node, sizeof (node));
	
	if(i2c_data.tx_is_idle)
	{
		TWCR |= (1 << TWSTA);
	}
	
	return status;	
}

void bg_process_I2C(void)
{
	I2C_Node current_node;
	
	while(!dequeue(&(i2c_data.rx_queue), (uint8_t*) & current_node, sizeof(current_node)))
	{
		if(current_node.callback != NULL)
		{
			current_node.callback(current_node);
		}
	}
}

ISR(TWI_vect)
{
	static uint8_t data_index;
	static uint8_t sub_addr_sent;
	static I2C_Node current_node;
	I2C_STATE status;
	
	status = (TWSR & I2C_STATUS_MASK);
	
	switch(status)
	{
		case I2C_START:
			TWCR &= ~(1 << TWSTA); //clear the start bit
			//try to get a new i2c node to send
			if(!dequeue(&(i2c_data.tx_queue), (uint8_t*) & current_node, sizeof (current_node))) //if nothing to send
			{
				TWCR |= (1 << TWINT) | (1 << TWSTO); //send the stop signal, clean the bus
				i2c_data.tx_is_idle = TRUE;
			}
			else //there is something to send
			{
				TWDR = get_write_addr(current_node.device_address);
				TWCR |= (1 << TWINT);
			}
			break;
			
		case I2C_RESTART:
			TWCR &= ~(1 << TWSTA); //clear the start bit
			if (current_node.mode == READ && sub_addr_sent == TRUE)
			{
				TWDR = get_read_addr(current_node.device_address);
			}
			else
			{
				TWDR = get_write_addr(current_node.device_address);
			}
			
			data_index = 0;
			TWCR |= (1 << TWINT);
			break;
			
		case I2C_MT_SLA_ACK: //send the sub address
			TWDR = current_node.sub_address;
			TWCR |= (1 << TWINT);
			sub_addr_sent = TRUE;
			break;
		
		case I2C_MT_DATA_ACK: //sub address has been sent
			if (current_node.mode == READ) //we are reading
			{
				TWCR |= (1 << TWINT) | (1 << TWSTA); //send the restart signal
			}
			else //we are writing
			{
				if (data_index != current_node.data_size) //if haven't sent all data
				{
					//send more data
					TWDR = current_node.data_buffer[data_index];
					TWCR |= (1 << TWINT);
				}
				else //we are done writing, done for the next transaction
				{
					//check for more nodes
					if(dequeue(&(i2c_data.tx_queue), (uint8_t*) & current_node, sizeof (current_node))) //load next node from the queue
					{
						TWCR |= (1 << TWINT) | (1 <<TWSTO); //send the stop bit
						i2c_data.tx_is_idle = TRUE; //flag that the bus is idle
					}
				}
			}
	}
	
}
