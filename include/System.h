/*
 * System.h
 *
 * Created: 11/25/2015 2:52:15 PM
 *  Author: james
 */ 


#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>

#define NULL (0)

typedef enum
{
	FALSE,
	TRUE
}bool;

typedef enum
{
	ERR_NO_ERR, //not an error
	
	//queue errors
	ERR_QUEUE_INSUF_ROOM,
	ERR_QUEUE_INSUF_DATA,
	
	//general errors
	ERR_BAD_CONFIG
}Error;



#endif /* SYSTEM_H_ */
