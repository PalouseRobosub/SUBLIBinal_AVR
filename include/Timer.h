/*
 * Timer.h
 *
 * Created: 11/25/2015 3:02:12 PM
 *  Author: james
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#include "System.h"

typedef enum
{
	TIMER_0,
	TIMER_1,
	TIMER_2
	
} Timer_Type;


typedef struct TIMER_CONFIG
{
	Timer_Type which_timer;
	uint32_t frequency;
	uint32_t pbclk;
	void (*callback);
	uint8_t enabled;
} Timer_Config;

Error initialize_timer(Timer_Config config);
Error enable_timer(Timer_Type which_timer);
Error disable_timer(Timer_Type which_timer);

#endif /* TIMER_H_ */