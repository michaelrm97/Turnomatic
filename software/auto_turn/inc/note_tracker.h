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
Bar_t track_song(void);

Bar_t track_set_bar(void);

#endif /* NOTE_TRACKER_H_ */
