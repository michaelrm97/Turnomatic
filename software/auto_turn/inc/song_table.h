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

void song_table_init(void);

char *song_name_get(int n);
int song_list_get(char (*names)[MAX_SONG_LEN], int max, int offset);
int song_num(void);

Song song_load(int n);
_U32 song_store(_U16 num_chords, Bar_t *page_breaks, char *name);
bool song_delete(int n);

#endif /* SONG_TABLE_H_ */
