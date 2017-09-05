/*
 * sct.h
 *
 *  Created on: 23 Aug 2017
 *      Author: Michael
 */

#ifndef SCT_H_
#define SCT_H_

#include <compiler.h>

// Use SCT_LOW for ADC
// Use SCT_HIGH for PWM

void sct_init(void);

void sct_set_period(_U32 period);
void sct_unset_period(void);


void sct_pwm_init(_U32 frequency);
void sct_pwm_stop(void);
void sct_pwm_pinassign(_U08 port, _U08 pin, _U08 channel);
void sct_pwm_setPinDutyCycle(_U08 channel, float duty);

#endif /* SCT_H_ */
