// Representation of a song

/*
   Represent song as a series of 'chords'
   Each chord is an array of notes
   During of note in semiquavers and bar number are also stored
*/

#ifndef SONG_H
#define SONG_H

#include <stdint.h>
#include "notes.h"

#define MAX_NOTES_IN_CHORD 4
#define MAX_CHORDS_IN_SONG 4096
#define MAX_TURNS 4
#define MAX_PAGES 5 // Number of pages (2 sheets side by side)
// Equal to MAX_TURNS + 1

typedef uint8_t Duration_t;
typedef uint16_t Bar_t;

typedef struct {
   Note_t notes;
   uint8_t n;
   Duration_t dur;
   Bar_t bar;
} Chord;

typedef struct {
   Chord *chords;
   Bar_t page_break[MAX_TURNS];
   uint16_t num_chords;
   uint16_t num_pages;
} Song;

extern Song song;

// Platform dependent function to load song from a file into ram
// On PC this would accept a filename
// On a uC this would accept an address in flash
// Returns 0 if successful, -1 if there is an error
int load_song(char *filename);

#endif