/*
 * Packetizer.c
 *
 * Created: 11/25/2015 3:05:52 PM
 *  Author: james
 */ 

#include "Packetizer.h"

Packetizer_Data packetizer_u1_data;

void initialize_packetizer(Packetizer_Config config)
{
	switch (config.which_channel)
	{
		case PACKET_UART_CH_1:
			packetizer_u1_data.control_byte = config.control_byte;
			packetizer_u1_data.receive_callback = config.callback;
			initialize_UART(config.uart_config);
			break;
	}
}

void send_packet(Data_Channel which_channel, uint8_t* data, uint8_t data_size)
{
	  switch (which_channel) 
	  {
		  case PACKET_UART_CH_1:
		  send_UART(&(packetizer_u1_data.control_byte), 1); //load control byte
		  send_UART(&(data_size), 1); //load size of packet
		  send_UART(data, data_size); //load packet data
		  break;
	  }
}

void bg_process_packetizer(Data_Channel which_channel)
{
	uint8_t current_byte;
	uint8_t status;

	void (*receive_callback) (uint8_t* data, uint8_t data_size); //receive callback function for the channel
	uint8_t* control_byte; //control byte for the channel
	uint8_t* sync_lock;
	uint8_t* packet_received;
	uint8_t* received_bytes;
	uint16_t* received_index;
	uint8_t* packet_length;

	//load data from queues
	//form packets by getting data from the queue
	//if a packet is sucessfully formed, do the callback
	//packetizer_callback(packet);

	//get one byte and setup the variables
	switch (which_channel) 
	{
		case PACKET_UART_CH_1:
			status = receive_UART(&current_byte, 1);
			receive_callback = packetizer_u1_data.receive_callback;
			control_byte = &packetizer_u1_data.control_byte;
			sync_lock = &packetizer_u1_data.sync_lock;
			packet_received = &packetizer_u1_data.packet_received;
			received_bytes = packetizer_u1_data.received_bytes; //array, so already pointer
			received_index = &packetizer_u1_data.received_index;
			packet_length = &packetizer_u1_data.packet_length;
			break;
	}

	if (status == 0) //if there were no errors we got a byte out of the buffer
	{
		if (*sync_lock) {
			switch (*received_index) {
				case 0: //first byte, should be control byte
				*packet_received = FALSE;
				++(*received_index);
				if (current_byte != *control_byte) { //if we lost sync
					*sync_lock = FALSE; //we fucked up
					*received_index = 0; //restart
				}
				break;
				case 1: //second byte, length byte
				*packet_length = current_byte;
				++(*received_index);
				break;

				default: //any other bytes are data bytes
				received_bytes[(*received_index) - 2] = current_byte; //store the received data
				++(*received_index);
				if ((*received_index)-2 == *packet_length) //we've received the entire packet
				{
					*packet_received = TRUE;
					*received_index = 0;
				}
				break;
			}

			} else { //we're out of sync, need to regain sync
			switch (*received_index) {
				case 0:
				if (current_byte == *control_byte) {
					*received_index = 1;
				}
				break;
				case 1:
				*packet_length = current_byte;
				++(*received_index);
				break;
				default:
				//absorb bytes until we have received the entire packet
				//receive one more, check for control byte
				//if control byte, then say we are in sync and prep to receive rest of packet
				++(*received_index);
				if (*received_index == *packet_length+3) //maybe length+3?
				{
					if (current_byte == *control_byte) //we appear to be in sync
					{
						*sync_lock = TRUE;
						*received_index = 1;
					}
				}
				break;
			}
		}

		if (*packet_received) //if we've receive a packet, process it!
		{
			receive_callback(received_bytes, *packet_length);
		}


	} //else, some error, don't do anything
	
}
