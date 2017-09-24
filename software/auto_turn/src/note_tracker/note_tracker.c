/*
 * note_tracker.c
 *
 *  Created on: 5 Sep 2017
 *      Author: Michael
 */

#include <pindefs.h>
#include <project_config.h>

#include <adc.h>

#include <note_tracker.h>
#include <filters.h>

Bar_t curr_bar = 1;
Page_t curr_page = 1;

void track_init(void) {

}

void track_set_song(Song s) {

}

void track_begin(void) {
	adc_set_periodic(F_SAMPLE, MIC_ADC_CHANNEL);
}

void track_stop(void) {

}

void ADC_SEQA_IRQHandler(void) {
	static int num = 0;
	num++;
}
