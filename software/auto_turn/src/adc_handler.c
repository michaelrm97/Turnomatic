/*
 * adc_handler.c
 *
 *  Created on: 5 Oct 2017
 *      Author: Michael
 */

#include <project_config.h>

#include <adc.h>
#include <mode.h>
#include <filters.h>

static _U16 pos_value;

_U16 pos_get_value(void) {
	return pos_value;
}

// Callback function upon completion of adc read
// Passes latest adc value into filter
void ADC_SEQA_IRQHandler(void) {
	Chip_ADC_ClearFlags(LPC_ADC, ADC_FLAGS_SEQA_INT_MASK);
	pos_value = ADC_DR_RESULT(Chip_ADC_GetDataReg(LPC_ADC, POT_ADC_CHANNEL));
	if (mode == MODE_PLAYING) {
		filter_input(ADC_DR_RESULT(Chip_ADC_GetDataReg(LPC_ADC, MIC_ADC_CHANNEL)));
	}
}

