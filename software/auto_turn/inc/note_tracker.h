/*
 * note_tracker.h
 *
 *  Created on: 5 Sep 2017
 *      Author: Michael
 */

#ifndef NOTE_TRACKER_H_
#define NOTE_TRACKER_H_

#include <compiler.h>

#include <song.h>

extern Bar_t curr_bar;
extern Bar_t max_bar;
extern Page_t curr_page;
extern Page_t max_page;

extern Note_t curr_note;

void track_init(void);

void track_reset_bar(void);
void track_increment_bar(void);
void track_decrement_bar(void);

void track_set_song(Song s);
void track_begin(void);
void track_update(void);
void track_stop(void);

#endif /* NOTE_TRACKER_H_ */
