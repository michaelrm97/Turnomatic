/*
 * song_table.h
 *
 *  Created on: 5 Sep 2017
 *      Author: Michael
 */

#ifndef SONG_TABLE_H_
#define SONG_TABLE_H_

#include <chip.h>
#include <compiler.h>

#include <song.h>

#define MAX_SONG_LEN 20

// Initialise song table
// Work out number of songs and used pages
void song_table_init(void);

// Get name of song at index n of song table
char *song_name_get(int n);

// Get list of song names
// Place songs into passed in array
// Places up to max names into array
// Starts from offset index
// Returns number of songs placed in array
int song_list_get(char (*names)[MAX_SONG_LEN], int max, int offset);

// Return song number and number of pages used
_U16 song_num(void);
_U16 song_used_pages(void);

// Load song at given index from table
Song song_load(int n);

// Store song with given parameters in song table
// Returns initial address to store song data
_U32 song_store(_U16 num_chords, Bar_t *page_break, char *name);

// Delete song from song table
// Returns whether delete was successful or not
bool song_delete(int n);

#endif /* SONG_TABLE_H_ */
