// Representation of a song

/*
   Represent song as a series of 'chords'
   Each chord is an array of notes
   During of note in semiquavers and bar number are also stored
*/

#include <stdio.h>
#include "song.h"

Chord song_chords[MAX_CHORDS_IN_SONG];
Song song;

// Platform dependent function to load song from a file into ram
// On PC this would accept a filename
// On a uC this would accept an address in flash
// Returns 0 if successful, -1 if there is an error

// On the uC- info about every song is stored in a single table

int load_song(char *filename) {

    song.chords = song_chords;
    return 0;
}