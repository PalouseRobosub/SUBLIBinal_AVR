/*
 * UART.h
 *
 * Created: 11/25/2015 3:02:42 PM
 *  Author: james
 */ 


#ifndef UART_H_
#define UART_H_

#include "System.h"
#include "Queue.h"

typedef struct UART_DATA {
	Queue rx_queue;
	Queue tx_queue; 
	uint8_t tx_is_idle;
} UART_Data;

typedef struct UART_CONFIG {
	uint32_t speed;
	uint32_t pb_clk;
	uint8_t *rx_buffer_ptr;
	uint16_t rx_buffer_size;
	uint8_t *tx_buffer_ptr;
	uint16_t tx_buffer_size;
	void (*rx_callback);
	void (*tx_callback);
	uint8_t tx_en;
	uint8_t rx_en;
} UART_Config;


Error initialize_UART(UART_Config config);
Error send_UART(uint8_t *data_ptr, uint16_t data_size);
Error receive_UART(uint8_t *data_ptr, uint16_t data_size);

#endif /* UART_H_ */