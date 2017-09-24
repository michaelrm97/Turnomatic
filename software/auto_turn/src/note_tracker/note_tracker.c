/*
 * note_tracker.c
 *
 *  Created on: 5 Sep 2017
 *      Author: Michael
 */

#include <pindefs.h>
#include <project_config.h>

#include <adc.h>
#include <adc_5410x.h>

#include <note_tracker.h>
#include <filters.h>

Bar_t curr_bar;
Bar_t max_bar;
Page_t curr_page;
Page_t max_page;

Song curr_song;

void track_init(void) {
	adc_pinassign(MIC_ADC_PORT, MIC_ADC_PIN);
}

void track_set_song(Song s) {
	curr_bar = 1;
	max_bar = 145;
	curr_page = 1;
	max_page = s.num_pages;
	curr_song = s;
}

void track_begin(void) {
	filter_reset();
	adc_set_periodic(F_SAMPLE, MIC_ADC_CHANNEL);
}

void track_stop(void) {
	adc_unset_periodic();
}

Note_t track_update(void) {
	Note_t notes[4];
	int num = filter_notes(notes, NULL, 4, 1e10);
	if (num > 0) {
		return notes[0];
	}
	return NO_NOTE;
}

void ADC_SEQA_IRQHandler(void) {
	Chip_ADC_ClearFlags(LPC_ADC, ADC_FLAGS_SEQA_INT_MASK);
	Sound_t val = ADC_DR_RESULT(Chip_ADC_GetDataReg(LPC_ADC, MIC_ADC_CHANNEL));
	filter_input(val);
}
