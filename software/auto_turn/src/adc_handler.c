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

#define NUM_POS 8

static _U16 pos_values[NUM_POS];
static int pos_index = 0;

_U16 pos_get_value(void) {
	_U32 result = 0;
	for (int i = 0; i < NUM_POS; i++) {
		result += pos_values[i];
	}
	return (_U16) (result / NUM_POS);
}

// Callback function upon completion of adc read
// Passes latest adc value into filter
void ADC_SEQA_IRQHandler(void) {
	Chip_ADC_ClearFlags(LPC_ADC, ADC_FLAGS_SEQA_INT_MASK);
	pos_values[pos_index++] = ADC_DR_RESULT(Chip_ADC_GetDataReg(LPC_ADC, POT_ADC_CHANNEL));
	pos_index = pos_index % NUM_POS;
	if (mode == MODE_PLAYING) {
		filter_input(ADC_DR_RESULT(Chip_ADC_GetDataReg(LPC_ADC, MIC_ADC_CHANNEL)));
	}
}

