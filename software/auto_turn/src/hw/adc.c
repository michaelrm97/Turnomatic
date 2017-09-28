/*
 * adc.c
 *
 *  Created on: 23 Aug 2017
 *      Author: Michael
 */

#include <adc.h>

#include <sct.h>

#include <adc_5410x.h>

// Initialise adc module
void adc_init(void) {

    sct_init();

	Chip_ADC_Init(LPC_ADC, (ADC_CR_RESOL(0x3) | ADC_CR_TSAMP(ADC_TSAMP_7CLK5)));

	LPC_ADC->STARTUP = 0x1;
	LPC_ADC->STARTUP = 0x3;

	Chip_ADC_Calibration(LPC_ADC);

	// Set sequencer A from SCT0-7
	// Set sequencer B to be manual

	Chip_ADC_SetupSequencer(LPC_ADC, ADC_SEQA_IDX, (ADC_SEQ_CTRL_TRIGGER(2) |
		ADC_SEQ_CTRL_HWTRIG_POLPOS | ADC_SEQ_CTRL_MODE_EOS));
	Chip_ADC_SetupSequencer(LPC_ADC, ADC_SEQB_IDX, ADC_SEQ_CTRL_MODE_EOS);
	Chip_ADC_EnableSequencer(LPC_ADC, ADC_SEQB_IDX);

}

// Deinitialise adc module
void adc_deinit(void) {
	Chip_ADC_DeInit(LPC_ADC);
}

// Set up a pin for analogue input mode
void adc_pinassign(_U08 port, _U08 pin) {
	Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, IOCON_INPFILT_OFF);
}

// Read one-off pin value for a particular channel
_U16 adc_readPin(_U08 channel) {
	// Do conversion of single channel
	Chip_ADC_ClearSequencerBits(LPC_ADC, ADC_SEQB_IDX, ADC_SEQ_CTRL_CHANSEL_MASK);
	Chip_ADC_SetSequencerBits(LPC_ADC, ADC_SEQB_IDX, ADC_SEQ_CTRL_CHANSEL(channel));

	Chip_ADC_EnableSequencer(LPC_ADC, ADC_SEQB_IDX);
	Chip_ADC_StartSequencer(LPC_ADC, ADC_SEQB_IDX);
	while(!ADC_DR_DONE(Chip_ADC_GetDataReg(LPC_ADC, channel)));
	Chip_ADC_DisableSequencer(LPC_ADC, ADC_SEQB_IDX);
	return ADC_DR_RESULT(Chip_ADC_GetDataReg(LPC_ADC, channel));

}

// Set up a periodic reading of a particular channel
void adc_set_periodic(_U32 frequency,_U08 channel) {
	Chip_ADC_ClearSequencerBits(LPC_ADC, ADC_SEQA_IDX, ADC_SEQ_CTRL_CHANSEL_MASK);
	Chip_ADC_SetSequencerBits(LPC_ADC, ADC_SEQA_IDX, ADC_SEQ_CTRL_CHANSEL(channel));

	sct_set_periodic(frequency);
	Chip_ADC_ClearFlags(LPC_ADC, ADC_FLAGS_SEQA_INT_MASK);
	Chip_ADC_EnableInt(LPC_ADC, (ADC_INTEN_SEQA_ENABLE));
	Chip_ADC_EnableSequencer(LPC_ADC, ADC_SEQA_IDX);
	NVIC_EnableIRQ(ADC_SEQA_IRQn);

}

// Unset periodic adc readings
void adc_unset_periodic(void) {
	sct_unset_periodic();
	Chip_ADC_DisableSequencer(LPC_ADC, ADC_SEQA_IDX);
	NVIC_ClearPendingIRQ(ADC_SEQA_IRQn);
	NVIC_DisableIRQ(ADC_SEQA_IRQn);
}

