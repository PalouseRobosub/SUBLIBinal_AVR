/*
 * Queue.c
 *
 * Created: 11/25/2015 3:06:37 PM
 *  Author: james
 */ 

#include "Queue.h"

Queue create_queue(uint8_t* buffer, uint16_t buffer_size)
{
	Queue queue;
	
	queue.buffer = buffer;
	queue.buffer_size = buffer_size;
	
	queue.queue_end = 0;
	queue.queue_start = 0;
	queue.num_stored = 0;
	
	return queue;
}
Error enqueue(Queue *queue, uint8_t *data, uint16_t data_size)
{
	uint16_t i, j;
	Error return_code = ERR_NO_ERR;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		if((queue->buffer_size - queue->num_stored) < data_size)
		{
				return_code = ERR_QUEUE_FULL;
		}
		else
		{
			for (i=0, j=queue->queue_end; i < data_size; ++i, ++j)
			{
				if (j == queue->buffer_size)
				{
					j = 0;
				}
				queue->buffer[j] = data[i];
			}
			
			queue->queue_end = j;
			queue->num_stored += data_size;		
		}
	}
	
	return return_code;
}
Error dequeue(Queue *queue, uint8_t *output_data, uint16_t data_size)
{
	uint16_t i, j;
	Error return_code = ERR_NO_ERR;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		if(data_size > queue->num_stored)
		{
			return_code = ERR_QUEUE_INVALID_READ;
		}
		else
		{
			for (i=0, j=queue->queue_start; i < data_size; ++i, ++j)
			{
				if (j == queue->buffer_size)
				{
					j = 0;
				}
				output_data[i] = queue->buffer[j];
			}
			
			queue->queue_start = j;
			queue->num_stored -= data_size;
		}
	}
	
	return return_code;
}
