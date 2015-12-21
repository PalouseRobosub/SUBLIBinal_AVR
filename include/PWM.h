/* 
 * File:   OC.h
 * Author: ryan
 *
 * Created on August 15, 2015, 9:20 PM
 */

#ifndef OC_H
#define	OC_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "System.h"
#include "Timer.h"
    
    typedef enum {
        PWM_CH_1, //Uses Pin PD5
		PWM_CH_1A_SIMPLE,
		PWM_CH_1B_SIMPLE,
        PWM_CH_2, //Uses Pin PB2
		PWM_CH_3, //Uses Pin PD3
		PWM_CH_3A_SIMPLE,
		PWM_CH_3B_SIMPLE
    }PWM_Channel;
    
    
    typedef struct {
        float dutyCycle; // Only used in PWM mode
        uint8 enable;
        PWM_Channel channel;
    }PWM_Config;

    
    //functions
    void initialize_PWM(PWM_Config config);
    void enable_PWM(PWM_Channel channel);
    void disable_PWM(PWM_Channel channel);
    
    void update_PWM(PWM_Config config, float dutyCycle);
    
    
    


#ifdef	__cplusplus
}
#endif

#endif	/* OC_H */

