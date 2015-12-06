/*
 * I2C.h
 *
 * Created: 11/25/2015 3:03:05 PM
 *  Author: james
 */ 


#ifndef I2C_H_
#define I2C_H_

#include "System.h"
#include "Queue.h"

#define I2C_SPEED (100000)

typedef enum
{
	READ,
	WRITE
} I2C_MODE;

typedef enum
{
	I2C_STATUS_MASK  = 0xF8,
	I2C_START        = 0x08,
	I2C_RESTART      = 0x10,
	I2C_MT_SLA_ACK   = 0x18,
	I2C_MT_DATA_ACK  = 0x28,
	I2C_MR_SLA_ACK   = 0x40,
	I2C_MR_DATA_ACK  = 0x50,
	I2C_MR_DATA_NACK = 0x58,
	I2C_BUSY         = 0xF8
	
} I2C_STATE;

#define get_read_addr(x) ((x << 1) | 0x01)
#define get_write_addr(x) ((x << 1))

typedef struct I2C_DATA 
{
	Queue rx_queue;
	Queue tx_queue;
	uint8_t tx_is_idle;
} I2C_Data;

typedef struct I2C_NODE 
{
	uint8_t device_id;
	uint8_t device_address;
	uint8_t sub_address;
	uint8_t* data_buffer;
	uint8_t data_size;
	I2C_MODE mode;
	void (*callback) (struct I2C_NODE);
} I2C_Node;

typedef struct I2C_CONFIG 
{
	uint32_t pb_clk;
	uint8_t *rx_buffer_ptr;
	uint16_t rx_buffer_size;
	uint8_t *tx_buffer_ptr;
	uint16_t tx_buffer_size;
	void* callback;
}I2C_Config;


Error initialize_I2C(I2C_Config config);
Error send_I2C(I2C_Node node);
void bg_process_I2C(void);


#endif /* I2C_H_ */
