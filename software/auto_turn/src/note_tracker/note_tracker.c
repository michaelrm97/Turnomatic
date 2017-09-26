/*
 * note_tracker.c
 *
 *  Created on: 5 Sep 2017
 *      Author: Michael
 */

#include <chip.h>

#include <pindefs.h>
#include <project_config.h>

#include <adc.h>
#include <adc_5410x.h>

#include <note_tracker.h>
#include <filters.h>
#include <motor.h>
#include <user.h>

Bar_t curr_bar;
Bar_t max_bar;
Page_t curr_page;
Page_t max_page;

Note_t curr_note;

Song curr_song;
_U32 curr_chord;

static bool note_in_chord(Note_t note, Note_t *notes, uint8_t n) {
	for (int i = 0; i < n; i++) {
		if (note == notes[i]) return TRUE;
	}
	return FALSE;
}

void track_init(void) {
	adc_pinassign(MIC_ADC_PORT, MIC_ADC_PIN);
}

void track_reset_bar(void) {
	curr_bar = 1;
	curr_page = 1;
	curr_chord = 0;
	motor_set_page(1);
}

void track_increment_bar(void) {
	if (curr_bar < max_bar) {
		curr_bar++;
		// Work out chord number
		while (curr_song.chords[curr_chord].bar < curr_bar) {
			curr_chord++;
		}
		if (curr_bar >= curr_song.page_break[curr_page - 1]) {
			curr_page++;
			// Set motor position
			motor_set_page(curr_page);
		}
	}
}

void track_decrement_bar(void) {
	if (curr_bar > 1) {
		curr_bar--;
		// Work out chord number
		while (curr_chord > 0 && curr_song.chords[curr_chord - 1].bar >= curr_bar) {
			curr_chord--;
		}
		if (curr_page > 1 && curr_bar < curr_song.page_break[curr_page - 2]) {
			curr_page--;
			// Set motor position
			motor_set_page(curr_page);
		}
	}
}

void track_set_song(Song s) {
	curr_bar = 1;
	max_bar = 145;
//	max_bar = s.chords[s.num_chords - 1].bar;
	curr_page = 1;
	max_page = s.num_pages;
	curr_song = s;
	curr_note = NO_NOTE;
	curr_chord = 0;
}

void track_begin(void) {
	filter_reset();
	adc_set_periodic(F_SAMPLE, MIC_ADC_CHANNEL);
}

void track_stop(void) {
	adc_unset_periodic();
}

void track_update(void) {
	Note_t notes[1];
	int num = filter_notes(notes, NULL, 1, MIC_THRESHOLD);
	if (num > 0) {
		curr_note = notes[0];
	} else {
		curr_note = NO_NOTE;
	}
	if (note_in_chord(curr_note, curr_song.chords[curr_chord].notes, curr_song.chords[curr_chord].n)) {
		curr_chord++;
		if (curr_chord < curr_song.num_chords) {
			curr_bar = curr_song.chords[curr_chord].bar;
			if (curr_bar >= curr_song.page_break[curr_page - 1]) {
				curr_page++;
				// Set motor position
				motor_set_page(curr_page);
			}
		} else {
			user_mode_set(MODE_PAUSED);
		}
	}

}

void ADC_SEQA_IRQHandler(void) {
	Chip_ADC_ClearFlags(LPC_ADC, ADC_FLAGS_SEQA_INT_MASK);
	Sound_t val = ADC_DR_RESULT(Chip_ADC_GetDataReg(LPC_ADC, MIC_ADC_CHANNEL));
	filter_input(val);
}
