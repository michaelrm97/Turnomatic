/*
 * note_tracker.c
 *
 *  Created on: 5 Sep 2017
 *      Author: Michael
 */

#include <pindefs.h>
#include <project_config.h>

#include <note_tracker.h>

#include <adc.h>
#include <adc_5410x.h>
#include <filters.h>
#include <motor.h>
#include <user.h>

#include <config_table.h>

#define NOTES_IN_OCTAVE 12
#define NEXT_CHORD_THRESH 4 // Number of notes played after skipped note to advance

Bar_t curr_bar;
Bar_t max_bar;
Page_t curr_page;
Page_t max_page;

Note_t curr_note;

static Song curr_song;
static _U32 curr_chord;
static _U32 next_chord;

// Determine whether a note is located in a chord
// Takes in target note, array of notes and number of notes in chord
static bool note_in_chord(Note_t note, Note_t *notes, uint8_t n) {
	if (n == 0) {
		return TRUE;
	}
	for (int i = 0; i < n; i++) {
		if ((note % NOTES_IN_OCTAVE) == (notes[i] % NOTES_IN_OCTAVE)) return TRUE;
	}
	return FALSE;
}

// Initialize note tracking module
void track_init(void) {
	adc_pinassign(MIC_ADC_PORT, MIC_ADC_PIN);
}

// Reset bar back to 1
void track_reset_bar(void) {
	curr_bar = 1;
	curr_page = 1;
	curr_chord = 0;
	next_chord = 1;
	motor_set_page(1);
}

// Increment bar number by 1
void track_increment_bar(void) {
	if (curr_bar < max_bar) {
		curr_bar++;
		// Work out chord number
		while (curr_song.chords[curr_chord].bar < curr_bar) {
			curr_chord++;
		}
		next_chord = curr_chord;
		if (next_chord + 1 < curr_song.num_chords) {
			next_chord++;
		}
		if (curr_page < max_page && curr_bar >= curr_song.page_break[curr_page - 1]) {
			curr_page++;
			motor_set_page(curr_page);
		}
	}
}

// Decrement bar number by 1
void track_decrement_bar(void) {
	if (curr_bar > 1) {
		curr_bar--;
		// Work out chord number
		while (curr_chord > 0 && curr_song.chords[curr_chord - 1].bar >= curr_bar) {
			curr_chord--;
		}
		next_chord = curr_chord;
		if (next_chord + 1 < curr_song.num_chords) {
			next_chord++;
		}
		if (curr_page > 1 && curr_bar < curr_song.page_break[curr_page - 2]) {
			curr_page--;
			motor_set_page(curr_page);
		}
	}
}

// Set song to track
void track_set_song(Song s) {
	curr_bar = 1;
	max_bar = s.chords[s.num_chords - 1].bar;
	curr_page = 1;
	max_page = s.num_pages;
	curr_song = s;
	curr_note = NO_NOTE;
	curr_chord = 0;
	next_chord = 1;
	motor_set_page(1);
}

// Begin tracking a song
void track_begin(void) {
	filter_reset();
}

// Update position in song based on last note played
void track_update(void) {
	if (curr_chord >= curr_song.num_chords) {
		user_mode_set(MODE_PAUSED);
		return;
	}
	Note_t notes[1];
	int num = filter_notes(notes, NULL, 1, MIC_THRESHOLD);
	if (num > 0) {
		curr_note = notes[0];
	} else {
		curr_note = NO_NOTE;
	}
	if (note_in_chord(curr_note, curr_song.chords[curr_chord].notes, curr_song.chords[curr_chord].n)) {
		curr_chord++;
		next_chord = curr_chord;
		if (next_chord + 1 < curr_song.num_chords) {
			next_chord++;
		}
		if (curr_chord < curr_song.num_chords) {
			curr_bar = curr_song.chords[curr_chord].bar;
			if (curr_page < max_page && curr_bar >= curr_song.page_break[curr_page - 1]) {
				curr_page++;
				// Set motor position
				motor_set_page(curr_page);
			}
		} else {
			user_mode_set(MODE_PAUSED);
		}
	} else if (note_in_chord(curr_note, curr_song.chords[next_chord].notes,
					curr_song.chords[next_chord].n)) {
		next_chord++;
		if (next_chord - curr_chord > NEXT_CHORD_THRESH || next_chord >= curr_song.num_chords) {
			curr_chord = next_chord;
			if (next_chord + 1 < curr_song.num_chords) {
				next_chord++;
			}
			if (curr_chord < curr_song.num_chords) {
				curr_bar = curr_song.chords[curr_chord].bar;
				if (curr_page < max_page && curr_bar >= curr_song.page_break[curr_page - 1]) {
					curr_page++;
					// Set motor position
					motor_set_page(curr_page);
				}
			} else {
				user_mode_set(MODE_PAUSED);
			}
		}
	}

}
