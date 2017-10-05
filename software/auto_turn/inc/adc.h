/*
 * adc.h
 *
 *  Created on: 23 Aug 2017
 *      Author: Michael
 */

#ifndef ADC_H_
#define ADC_H_

#include <chip.h>
#include <compiler.h>

// Initialize and deinitialize ADC module
void adc_init(void);
void adc_deinit(void);

// Set up a pin for analogue input mode
void adc_pinassign(_U08 port, _U08 pin);

// Set and unset a periodic reading of a particular channel
void adc_set_periodic(_U32 frequency, _U32 channel_mask);
void adc_unset_periodic(void);

#endif /* ADC_H_ */
