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

void sct_set_periodic(_U32 period);
void sct_unset_periodic(void);

#endif /* SCT_H_ */
