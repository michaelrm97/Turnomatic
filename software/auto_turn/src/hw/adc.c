/*
 * adc.c
 *
 *  Created on: 23 Aug 2017
 *      Author: Michael
 */

#include <adc.h>

#include <adc_5410x.h>

void adc_init(void) {
	Chip_ADC_Init(LPC_ADC, (ADC_CR_CLKDIV(1) | ADC_CR_RESOL(0x3) | TSAMP(0)));
}

void adc_deinit(void) {

}

void adc_pinassign(_U08 port, _U08 pin, _U08 channel) {

}

_U16 adc_readPin(_U08 channel) {

}

void adc_set_periodic(_U08 port, _U08 pin, _U08 channel, void (*handler)(void)) {

}

void adc_unset_periodic(void) {

}
