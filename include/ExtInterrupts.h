/* 
 * File:   ExtInterrupts.h
 * Author: Ryan
 *
 * Created on August 6, 2015, 5:42 PM
 */

#ifndef EXTINTERRUPTS_H
#define	EXTINTERRUPTS_H

#ifdef	__cplusplus
extern "C" {
#endif
    #include "System.h"

    typedef enum {
        INT0,
        INT1,
        PCINT0,
        PCINT1,
        PCINT2
    }Interrupt;

    
    typedef enum {
        falling,
        rising,
		changing
    } Polarity;
    
    typedef enum {
        none,
        pullup
    } Resistor;
    
    typedef struct INTERRUPT_CONFIG{
        Interrupt extInt;
        Pin pin;
        Polarity polarity;
        int enable;
        Resistor resistor;
        void *callback;
    } Interrupt_Config;
    
    void initialize_Interrupt(Interrupt_Config config);
    void disable_Interrupt(Interrupt extInt);
    void enable_Interrupt(Interrupt extInt);
    

#ifdef	__cplusplus
}
#endif

#endif	/* EXTINTERRUPTS_H */

