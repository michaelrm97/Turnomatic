/*
 * song_table.c
 *
 *  Created on: 5 Sep 2017
 *      Author: Michael
 */

#include <compiler.h>
#include <string.h>

#include <song_table.h>
#include <flash.h>

#include <user.h>

_U16 num_songs;
_U16 used_pages;

#define SONG_TABLE ((Song_Entry *)(SONG_TABLE_BASE))
#define SONG_DATA ((Chord_t *)(SONG_DATA_BASE))

typedef struct {
	char name[MAX_SONG_LEN];
	_U16 flash_page;
	_U16 num_chords;
	Bar_t page_break[MAX_TURNS];
} Song_Entry;

void song_table_init(void) {
	int i;
	for (i = 0; i < MAX_SONGS; i++) {
		if (SONG_TABLE[i].num_chords == 0) {
			break;
		}
	}
	num_songs = i;
	used_pages = SONG_TABLE[i - 1].flash_page - SONG_DATA_PAGE + 1;
}

char *song_name_get(int n) {
	return SONG_TABLE[n].name;
}

int song_list_get(char (*names)[MAX_SONG_LEN], int max, int offset) {
	int n = 0;
	while (offset < num_songs && n < max) {
		strcpy(names[n], SONG_TABLE[offset].name);
		offset++;
		n++;
	}
	return n;
}

_U16 song_num(void) {
	return num_songs;
}

_U16 song_used_pages(void) {
	return used_pages;
}

Song song_load(int n) {
	Song ret;
	ret.chords = (Chord *)(SONG_TABLE[n].flash_page * 256);
	ret.num_chords = SONG_TABLE[n].num_chords;
	int i = 0;
	for (i = 0; i < 4; i++) {
		if (!SONG_TABLE[i].page_break[i]) {
			break;
		}
		ret.page_break[i] = SONG_TABLE[n].page_break[i];
	}
	ret.num_pages = i + 1;
	return ret;
}

// Add song to table
// Returns address of initial flash memory page
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
	flash_copy(SONG_TABLE_BASE, SONG_TABLE_SIZE);
	num_songs++;
	used_pages += (num_chords + 31) >> 5;
	return (start_page << 8);
}

bool song_delete(int n) {
	if (n >= num_songs) {
		return FALSE;
	}
	_U32 del_pages = (SONG_TABLE[n].num_chords + 31) >> 5;
	if (n < num_songs - 1) {
		// Move all the data down
		_U32 start_page = SONG_TABLE[n + 1].flash_page;
		_U32 end_page = used_pages + SONG_DATA_PAGE;
		int work = end_page - start_page + 1;
		int complete = 0;
		while (start_page < end_page) {
			flash_read(start_page << 5, COPY_SIZE);
			flash_copy((start_page - del_pages) << 8, COPY_SIZE);
			start_page++;
			complete++;
			user_update_deleting(complete * 100 / work);
		}
	}
	flash_read(SONG_TABLE_BASE, SONG_TABLE_SIZE);
	Song_Entry *table = (Song_Entry *)flash_buffer;
	for (int i = n + 1; i < num_songs; i++) {
		table[i - 1] = table[i];
	}
	flash_copy(SONG_TABLE_BASE, SONG_TABLE_SIZE);
	num_songs--;
	used_pages -= del_pages;
	num_songs--;
	return TRUE;
}
