/*
 * song_table.c
 *
 *  Created on: 5 Sep 2017
 *      Author: Michael
 */

#include <compiler.h>

#include <song_table.h>
#include <string.h>

int num_songs;
int space_used;

typedef struct {
	char name[MAX_SONG_LEN];
	_U16 flash_page;
	_U16 num_chords;
	Bar_t page_break[MAX_TURNS];
} Song_Entry;

// Dummy table
#define NUM_SONGS 12
Song_Entry dummy_table[NUM_SONGS] = {
		{"Twinkle Star", 0, 0, {0, 0, 0, 0}},
		{"Clocks", 0, 0, {0, 0, 0, 0}},
		{"Hamilton", 0, 0, {0, 0, 0, 0}},
		{"La La Land", 0, 0, {0, 0, 0, 0}},
		{"Sound of Silence", 0, 0, {0, 0, 0, 0}},
		{"Mad World", 0, 0, {0, 0, 0, 0}},
		{"All Star", 0, 0, {0, 0, 0, 0}},
		{"Turkish March", 0, 0, {0, 0, 0, 0}},
		{"Fur Elise", 0, 0, {0, 0, 0, 0}},
		{"Eine Kleine", 0, 0, {0, 0, 0, 0}},
		{"Hung. Dance 9", 0, 0, {0, 0, 0, 0}},
		{"Spring", 0, 0, {0, 0, 0, 0}}
};

void song_table_init(void) {
	num_songs = NUM_SONGS;
}

char *song_name_get(int n) {
	return dummy_table[n].name;
}

int song_list_get(char (*names)[MAX_SONG_LEN], int max, int offset) {
	int n = 0;
	while (offset < num_songs && n < max) {
		strcpy(names[n], dummy_table[offset].name);
		offset++;
		n++;
	}
	return n;
}

int song_num(void) {
	return num_songs;
}

int song_free_space(void) {
	return 0;
}

Song song_load(int n) {
	Song ret;
	ret.chords = (Chord *)(dummy_table[n].flash_page * 256);
	ret.num_chords = dummy_table[n].num_chords;
	int i = 0;
	for (i = 0; i < 4; i++) {
		if (!dummy_table[i].page_break[i]) {
			break;
		}
		ret.page_break[i] = dummy_table[n].page_break[i];
	}
	ret.num_pages = i + 1;
	return ret;
}

void song_store(void) {

}

void song_delete(int n) {

}
