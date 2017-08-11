// Filter bank used to estimate notes

#ifndef FILTERS_H
#define FILTERS_H

#include <stdint.h>
#include "notes.h"

typedef int16_t Sound_t; // 16 bits for sound input
typedef float Filter_t; // Filter values
typedef float Power_t; // Average power of filter


#define F_SAMPLE 16000 // Sample rate
#define N_FILTER 2048 // Comb filter order
#define A_FILTER 0.9999 // Poles slightly inside unit circle

#define COMB_AN 0.8148019179981028
#define RES_A1  2
#define RES_B2  0.9998000100000001

#define MIN_NOTE C2
#define MAX_NOTE B7
#define R_NOTE MAX_NOTE - MIN_NOTE + 1

// a2 and b1 coefficients of filter
extern const float filter_coeff[R_NOTE];

// Reset all filters (0 initialize)
void filter_reset(void);

// Add a sound value to the filter input
void filter_input(Sound_t s);

// Get current power of specified note
Power_t get_power(Note_t note);

// Finds the notes with the maximal power values
uint8_t filter_notes(Note_t *notes, Power_t *powers, uint8_t num, float threshold);

#endif

