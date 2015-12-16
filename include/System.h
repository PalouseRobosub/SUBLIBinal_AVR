/*!
 * \file System.h
 * The main include header for the library
 */

#ifndef SYSTEM_H
#define	SYSTEM_H

#ifdef	__cplusplus
extern "C" {
#endif


//If the type is not defined as arduino, we are compiling for AVR studio. Include the necessary files
#ifndef _ARDUINO
	#include <avr/io.h>
	#include <inttypes.h>
	#include <avr/interrupt.h>
#endif

#ifndef NULL
    #define NULL 0
#endif


//To compile the library for Arduino, define _ARDUINO in the source code before you compile
#ifdef _ARDUINO
	#include "Arduino.h"
#endif

/* Type Defintions*/
    typedef unsigned long uint; ///< An unsigned 32-bit integer
    typedef unsigned int uint16; ///< An unsigned 16-bit integer
    typedef unsigned char uint8; ///< An unsigned 8-bit integer
    typedef signed char sint8; ///< A signed 8-bit integer

	typedef enum {
		FALSE,
		TRUE
	} Boolean;



	/// \enum Error Defines the possible communcation errors
    typedef enum {
        ERR_NO_ERR, //not an error
        ERR_INVALID_ENUM, //invalid enumeration supplied to function
        //Timer Errors
        ERR_TIMER_FREQ_UNATTAINABLE,
        ERR_INVALID_DIVIDER,
        ERR_INVALID_PERIOD,
                
		ERR_BAD_CONFIG,                
        ERR_INVALID_SEND,
        ERR_QUEUE_FULL, //queue is full
        ERR_QUEUE_INVALID_READ,
        ERR_INVALID_CHANNEL //invalid channel
    }Error;
    
    void disable_Interrupts();
    void enable_Interrupts();

#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_H */

