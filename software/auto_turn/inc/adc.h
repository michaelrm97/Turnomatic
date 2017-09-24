/*
 * adc.h
 *
 *  Created on: 23 Aug 2017
 *      Author: Michael
 */

#ifndef ADC_H_
#define ADC_H_

#include <compiler.h>

#include <chip.h>

void adc_init(void);
void adc_deinit(void);

void adc_pinassign(_U08 port, _U08 pin);
_U16 adc_readPin(_U08 channel);

void adc_set_periodic(_U32 frequency, _U08 channel);
void adc_unset_periodic(void);

#endif /* ADC_H_ */
