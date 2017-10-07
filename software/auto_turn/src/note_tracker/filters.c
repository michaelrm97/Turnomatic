// Filter bank used to estimate notes

#include <chip.h>
#include <filters.h>

static Sound_t filter_values[N_FILTER]; // Last N_FILTER values of input
static Filter_t comb[N_COMB];           // Last N_COMB comb filter outputs
static Filter_t res[R_NOTE][N_RES];     // Last N_RES resonator output values for each note
static Power_t powers[R_NOTE][N_POWER]; // Last N_POWER power values for each note
static Power_t curr_powers[R_NOTE];     // Current sum of powers- gives rolling average for each note

static int filter_index;
static int comb_index;
static int res_index;
static int powers_index;

// Reset all filters (0 initialize everything)
void filter_reset(void) {

    for (int i = 0; i < N_FILTER; i++) {
        filter_values[i] = 0;
    }

    for (int i = 0; i < N_COMB; i++) {
        comb[i] = 0;
    }

    for (int i = 0; i < R_NOTE; i++) {
        for (int j = 0; j < N_RES; j++) {
            res[i][j] = 0;
        }
    }

    for (int i = 0; i < R_NOTE; i++) {
        for (int j = 0; j < N_POWER; j++) {
            powers[i][j] = 0;
        }
        curr_powers[i] = 0;
    }

    filter_index = 0;
    comb_index = 0;
    res_index = 0;
    powers_index = 0;

}

// Add a sound value to the filter input
void filter_input(Sound_t s) {

    // Comb filter
    comb[comb_index] = s - COMB_AN * filter_values[filter_index];
    filter_values[filter_index] = s;

    // Resonators
    int cj = comb_index - 1 >= 0 ? comb_index - 1 : N_COMB - 1;
    int rj = res_index - 1 >= 0 ? res_index - 1 : N_RES - 1;
    int rk = rj - 1 >= 0 ? rj - 1 : N_RES - 1;

    for (int i = 0; i < R_NOTE; i++) {
        res[i][res_index] = RES_A1 * comb[comb_index] + filter_coeff[i] * comb[cj] -
            filter_coeff[i] * res[i][rj] - RES_B2 * res[i][rk];
        Power_t curr = res[i][res_index] * res[i][res_index];
        curr_powers[i] -= powers[i][powers_index];
        powers[i][powers_index] = curr;
        curr_powers[i] += curr;
    }

    filter_index = filter_index + 1 < N_FILTER ? filter_index + 1 : 0;
    comb_index = comb_index + 1 < N_COMB ? comb_index + 1 : 0;
    res_index = res_index + 1 < N_RES ? res_index + 1 : 0;
    powers_index = powers_index + 1 < N_POWER ? powers_index + 1 : 0;

}

// Get current power of specified note
Power_t get_power(Note_t note) {
    if (note < MIN_NOTE || note > MAX_NOTE) {
        return -1; // Note outside of range
    }
    return curr_powers[note - MIN_NOTE];
}

// Finds the notes with the maximal power values
// Returns number of notes with power above a given threshold
// Returns notes and power by reference
// num is the size of the notes and powers arrays
uint8_t filter_notes(Note_t *notes, Power_t *powers, uint8_t num, float threshold) {
    Power_t sort[num];
    uint8_t count = 0;
    for (int i = 0; i < R_NOTE; i++) {
        if (curr_powers[i] >= threshold) {
            // Check whether note is in the top num of values
            int added = 0;
            int j;
            for (j = 0; j < count && !added; j++) {
                if (curr_powers[i] > sort[j]) {
                    // Insert note
                    for (int k = count - 1; k >= j; k--) {
                        if (k + 1 < num) {
                            notes[k + 1] = notes[k];
                            sort[k + 1] = sort[k];
                        }
                    }
                    notes[j] = i + MIN_NOTE;
                    sort[j] = curr_powers[i];
                    added = 1;
                    if (count < num) {
                        count++;
                    }
                }
            }
            // Add to end
            if (!added && j == count && count < num) {
                notes[j] = i + MIN_NOTE;
                sort[j] = curr_powers[i];
                count++;
            }
        }
    }
    if (powers != NULL) {
        for (int i = 0; i < count; i++) {
            powers[i] = sort[i];
        }
    }
    return count;
}
