/*
 * song_table.c
 *
 *  Created on: 5 Sep 2017
 *      Author: Michael
 */

#include <cr_section_macros.h>

#include <song_table.h>
#include <flash.h>

#include <user.h>
#include <usb.h>

#include <string.h>

static _U16 num_songs;
static _U16 used_pages;

#define SONG_TABLE ((Song_Entry *)(SONG_TABLE_BASE))
#define SONG_DATA ((Chord_t *)(SONG_DATA_BASE))

typedef struct {
	char name[MAX_SONG_LEN];
	_U16 flash_page;
	_U16 num_chords;
	Bar_t page_break[MAX_TURNS];
} Song_Entry;

// Initial song table
__RODATA(Flash2) const Song_Entry song_table[MAX_SONGS] = {0};

// Initialise song table
// Work out number of songs and used pages
void song_table_init(void) {
	int i;
	for (i = 0; i < MAX_SONGS; i++) {
		if (song_table[i].num_chords == 0) {
			break;
		}
	}
	num_songs = i;
	if (num_songs > 0) {
		used_pages = song_table[num_songs - 1].flash_page - SONG_DATA_PAGE +
				((song_table[num_songs - 1].num_chords + 31) >> 5);
	} else {
		used_pages = 0;
	}
}

// Get name of song at index n of song table
char *song_name_get(int n) {
	return song_table[n].name;
}
// Get list of song names
// Place songs into passed in array
// Places up to max names into array
// Starts from offset index
// Returns number of songs placed in array
int song_list_get(char (*names)[MAX_SONG_LEN], int max, int offset) {
	int n = 0;
	while (offset < num_songs && n < max) {
		strcpy(names[n], song_table[offset].name);
		offset++;
		n++;
	}
	return n;
}

// Return song number
_U16 song_num(void) {
	return num_songs;
}

// Return number of pages used by song data
_U16 song_used_pages(void) {
	return used_pages;
}

// Load song at given index from table
Song song_load(int n) {
	Song ret;
	ret.chords = (Chord *)(song_table[n].flash_page * 256);
	ret.num_chords = song_table[n].num_chords;
	int i = 0;
	for (i = 0; i < 4; i++) {
		if (!song_table[n].page_break[i]) {
			break;
		}
		ret.page_break[i] = song_table[n].page_break[i];
	}
	ret.num_pages = i + 1;
	return ret;
}

// Store song with given parameters in song table
// Returns initial address to store song data
_U32 song_store(_U16 num_chords, Bar_t *page_break, char *name) {
	// Find first free page
	_U32 start_page = SONG_DATA_PAGE + used_pages;
	// Copy into buffer
	flash_read(SONG_TABLE_BASE, SONG_TABLE_SIZE);
	Song_Entry *table = (Song_Entry *)flash_buffer;
	table[num_songs].flash_page = start_page;
	strncpy(table[num_songs].name, name, MAX_SONG_LEN);
	table[num_songs].num_chords = num_chords;
	for (int i = 0; i < 4; i++) {
		table[num_songs].page_break[i] = page_break[i];
	}
	// Work out which page to write
	_U32 offset = (num_songs << 5) & ~(0xFF);

	flash_copy(SONG_TABLE_BASE + offset, offset, 256);
	num_songs++;
	used_pages += (num_chords + 31) >> 5;
	return (start_page << 8);
}

// Delete song from song table
// Returns whether delete was successful or not
bool song_delete(int n) {
	if (n >= num_songs) {
		return FALSE;
	}
	_U32 del_pages = (SONG_TABLE[n].num_chords + 31) >> 5;
	_U32 start = (n << 5) & ~(0xFF);
	_U32 end = ((num_songs - 1) << 5) & ~(0xFF);
	int work = ((end - start) >> 5) + 1; // number of pages to write in total
	int complete = 0;
	if (n < num_songs - 1) {
		// Move all the data down
		_U32 start_page = song_table[n + 1].flash_page;
		_U32 end_page = used_pages + SONG_DATA_PAGE;
		work += end_page - start_page;
		while (start_page < end_page) {
			flash_read(start_page << 5, COPY_SIZE);
			flash_copy((start_page - del_pages) << 8, 0, COPY_SIZE);
			start_page++;
			complete++;
			user_update_loading(complete * 100 / work);
			usb_ack();
		}
	}
	flash_read(SONG_TABLE_BASE, SONG_TABLE_SIZE);
	Song_Entry *table = (Song_Entry *)flash_buffer;
	for (int i = n + 1; i < num_songs; i++) {
		table[i - 1] = table[i];
	}
	memset(&table[num_songs - 1], 0, sizeof(Song_Entry));
	while (start <= end) {
		flash_copy(SONG_TABLE_BASE + start, start, 256);
		start += 256;
		complete++;
		user_update_loading(complete * 100 / work);
		usb_ack();
	}
	num_songs--;
	used_pages -= del_pages;
	return TRUE;
}
