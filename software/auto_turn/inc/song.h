// Representation of a song

/*
   Represent song as a series of 'chords'
   Each chord is an array of notes
   During of note in semiquavers and bar number are also stored
*/

#ifndef SONG_H
#define SONG_H

#include <project_config.h>

#include <notes.h>

typedef uint8_t Duration_t;
typedef uint8_t Page_t;
typedef uint16_t Bar_t;

typedef struct {
   Note_t notes[MAX_NOTES_IN_CHORD];
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

#endif
