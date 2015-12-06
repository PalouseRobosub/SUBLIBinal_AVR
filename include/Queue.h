/*
 * Queue.h
 *
 * Created: 11/25/2015 3:02:00 PM
 *  Author: james
 */ 


#ifndef QUEUE_H_
#define QUEUE_H_

#include "System.h"
#include <util/atomic.h>


//structure definitions
typedef struct QUEUE
{
	uint8_t *buffer;
	uint16_t buffer_size;
	uint16_t queue_start;
	uint16_t queue_end;
	uint16_t num_stored;
}Queue;

Queue create_queue(uint8_t* buffer, uint16_t buffer_size);
Error enqueue(Queue *queue, uint8_t *data, uint16_t data_size);
Error dequeue(Queue *queue, uint8_t *output_data, uint16_t data_size);


#endif /* QUEUE_H_ */