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

void track_init(void);

Bar_t track_get_bar(void);

void track_song(Song s);
void track_set_bar(Bar_t b);

#endif /* NOTE_TRACKER_H_ */
