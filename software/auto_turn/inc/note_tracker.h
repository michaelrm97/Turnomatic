/*
 * note_tracker.h
 *
 *  Created on: 5 Sep 2017
 *      Author: Michael
 */

#ifndef NOTE_TRACKER_H_
#define NOTE_TRACKER_H_

#include <chip.h>
#include <compiler.h>

#include <song.h>

extern Bar_t curr_bar;
extern Bar_t max_bar;
extern Page_t curr_page;
extern Page_t max_page;

extern Note_t curr_note;

// Initialize note tracking module
void track_init(void);

// Reset, increment and decrement bar
// Determines required changes in chord and page number
void track_reset_bar(void);
void track_increment_bar(void);
void track_decrement_bar(void);

// Set song to track
void track_set_song(Song s);

// Begin and stop tracking of song and update song location
// Based on last note played
void track_begin(void);
void track_update(void);
void track_stop(void);

#endif /* NOTE_TRACKER_H_ */
