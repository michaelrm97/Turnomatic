/*
 * song_table.c
 *
 *  Created on: 5 Sep 2017
 *      Author: Michael
 */

#include <compiler.h>

#include <song_table.h>

#define MAX_SONG_LEN 20

int num_songs;
int space_used;

typedef struct {
	char name[MAX_SONG_LEN];
	_U16 flash_page;
	_U16 num_chords;
	Bar_t page_break[MAX_TURNS];
} Song_Entry;
