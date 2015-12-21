/*
 * Packetizer.h
 *
 * Created: 11/25/2015 3:03:30 PM
 *  Author: james
 */ 


#ifndef PACKETIZER_H_
#define PACKETIZER_H_

#include "System.h"
#include "UART.h"

typedef enum {
	PACKET_UART_CH_1
} Data_Channel;

typedef struct PACKETIZER_DATA {
	void (*receive_callback) (uint8_t* data, uint8_t data_size);
	uint8_t control_byte;
	uint8_t sync_lock;
	uint8_t packet_received;
	uint8_t received_bytes[255];
	uint16_t received_index;
	uint8_t packet_length;
} Packetizer_Data;

typedef struct PACKETIZER_CONFIG {
	Data_Channel which_channel;
	uint8_t control_byte;
	void (*callback) (uint8_t* data, uint8_t data_size);
	UART_Config uart_config;
}Packetizer_Config;


void initialize_packetizer(Packetizer_Config config);
void send_packet(Data_Channel which_channel, uint8_t* data, uint8_t data_size);
void bg_process_packetizer(Data_Channel which_channel);

#endif /* PACKETIZER_H_ */
