/*
 * song_table.h
 *
 *  Created on: 5 Sep 2017
 *      Author: Michael
 */

#ifndef SONG_TABLE_H_
#define SONG_TABLE_H_

#include <compiler.h>
#include <song.h>

#define MAX_SONG_LEN 20

void song_table_init(void);

char *song_name_get(int n);
int song_list_get(char (*names)[MAX_SONG_LEN], int max, int offset);
int song_num(void);
int song_free_space(void);

Song song_load(int n);
void song_store(void);
void song_delete(int n);

#endif /* SONG_TABLE_H_ */
