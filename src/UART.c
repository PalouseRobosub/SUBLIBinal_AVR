/*
 * UART.c
 *
 * Created: 11/25/2015 3:07:05 PM
 *  Author: james
 */ 

#include "UART.h"

void (*uart_1_tx_callback) (void);
void (*uart_1_rx_callback) (void);

UART_Data u1;

Error initialize_UART(UART_Config config)
{

	//set the baud rate
	UBRR0 = config.pb_clk/(16*config.speed) - 1;
			
	if (config.rx_en) //enable rx interrupt and receiver
	{
		UCSR0B |= (1 << RXCIE0) | (1 << RXEN0); 
	}
	if(config.tx_en) //enable tx interupt and transmitter
	{
		UCSR0B |= (1 << UDRIE0) | (1 << TXEN0);
	}
			
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); //8 bit operation
			
	//setup the rx and tx buffers
	u1.rx_queue = create_queue(config.rx_buffer_ptr, config.rx_buffer_size);
	u1.tx_queue = create_queue(config.tx_buffer_ptr, config.tx_buffer_size);
	u1.tx_is_idle = 0;
	
	return ERR_NO_ERR;
}

Error send_UART(uint8_t *data_ptr, uint16_t data_size)
{
	Error status = ERR_NO_ERR;
	
	status = enqueue(&(u1.tx_queue), data_ptr, data_size);
	if (u1.tx_is_idle) { //if the tx is idle, force-start it
		UCSR0B |=  (1 << UDRIE0);
	}
	
	return status;
}

Error receive_UART(uint8_t *data_ptr, uint16_t data_size)
{
	Error status = ERR_NO_ERR;
	
	status = dequeue(&(u1.rx_queue), data_ptr, data_size);
	
	return status;
}

//Tx ISR
ISR(USART_UDRE_vect) 
{
	uint8_t transmit;
	
	u1.tx_is_idle = 0; //tx is not idle
	
	if (u1.tx_queue.num_stored == 0) //no more data to send
	{
		UCSR0B &=  ~(1 << UDRIE0); //disable tx interrupts so it stops firing
		u1.tx_is_idle = 1; //flag that the tx is idle
	} 
	else 
	{
		dequeue(&(u1.tx_queue), &transmit, 1);
		
		UDR0 = transmit;
	}
	
	if (uart_1_tx_callback != NULL)
	{
		uart_1_tx_callback(); //call additional ISR functionality
	}
	
	//now clear the interrupt flag
	UCSR0A &= ~(1 << UDRE0);
}

//Rx ISR
ISR(USART_RX_vect)
{
	uint8_t received;
		
	received = UDR0; //pull byte from rx register
	enqueue(&(u1.rx_queue), &received, 1); //store in buffer

	if (uart_1_rx_callback != NULL) 
	{
		uart_1_rx_callback(); //call additional ISR functionality
	}

	//now clear the interrupt flag
	UCSR0A &= ~(1 << RXC0);
}