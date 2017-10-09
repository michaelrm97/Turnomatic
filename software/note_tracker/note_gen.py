import sys, math

def gen_notes():
    notes = ['C', 'CS', 'D', 'Eb', 'E', 'F', 'FS', 'G', 'GS', 'A', 'Bb', 'B']
    j = 0
    x = []
    for i in range(-1, 10):
        for n in notes:
            if i < 0:
                s = '#define %sn%d' % (n, -i)
            else:
                s = '#define %s%d' % (n, i)
            s += ' ' * (15 - len(s))
            s += str(j)
            x.append(s)
            j = j + 1
            if j  > 127:
                break
    return x

def gen_names():
    notes = ['C', 'CS', 'D', 'DS', 'E', 'F', 'FS', 'G', 'GS', 'A', 'AS', 'B']
    j = 0
    names = []
    for i in range(-1, 10):
        for n in notes:
            if i < 0:
                names.append('%sn%d' % (n, -i))
            else:
                names.append('%s%d' % (n, i))
            j = j + 1
            if j > 127:
                break
    return names

def gen_freq():
    f = 440
    freq = [0] * 128
    for i in range(69, -1, -1):
        freq[i] = round(f, 2)
        f = f / (2 ** (1/float(12)))
    f = 440
    for i in range(69, 128):
        freq[i] = round(f, 2)
        f = f * (2 ** (1/float(12)))
    return freq

def gen_coeff(Fs, N, a, min, max):
    coeff = []
    freq = gen_freq()
    names = gen_names()
    for i in range(names.index(min), names.index(max) + 1):
        ni = round(freq[i] / Fs * N)
        coeff.append(-2 * a * math.cos(2 * math.pi * ni / N))
    return coeff

if __name__ == '__main__':
    if '--notes' in sys.argv:
        for line in gen_notes():
            print(line)
    elif '--freq' in sys.argv:
        for n in gen_freq():
            print('%.2f' % n)
    elif '--names' in sys.argv:
        for line in gen_names():
            print(line)
    elif '--coeff' in sys.argv:
        try:
            max = sys.argv.pop()
            min = sys.argv.pop()
            a = float(sys.argv.pop())
            N = int(sys.argv.pop())
            Fs = int(sys.argv.pop())
        except:
            print('Usage ./note_gen.py --coeff Fs N a min max', file=sys.stderr)
            sys.exit(1)
        for line in gen_coeff(Fs, N, a, min, max):
            print(line)
    elif '--notes-h' in sys.argv:
        print("""// Definition of each note

#ifndef NOTES_H
#define NOTES_H

#include <stdint.h>

// Define each note as its midi number
typedef uint8_t Note_t;

#define N_NOTES 128 // Number of defined notes
""")
        for line in gen_notes():
            print(line)
        print("""
// Cn1 is below the frequency range of what we can actually measure
// Use this to indicate no note
#define NO_NOTE 0
""")
        print("""extern const char * note_names[N_NOTES];
extern const float note_freq[N_NOTES];

#endif
""")
    elif '--notes-c' in sys.argv:
        print("""// Note definitions

#include "notes.h"

const char * note_names[N_NOTES] = {""")
        print('  ',',\n   '.join(['"%s"' % n for n in gen_names()]))
        print("""};

const float note_freq[N_NOTES] = {""")
        print('  ',',\n   '.join(['%.2f' % n for n in gen_freq()]))
        print('};')
    elif '--filters-h' in sys.argv:
        try:
            max = sys.argv.pop()
            min = sys.argv.pop()
            a = float(sys.argv.pop())
            N = int(sys.argv.pop())
            Fs = int(sys.argv.pop())
        except:
            print('Usage ./note_gen.py --filters-h Fs N a min max', file=sys.stderr)
            sys.exit(1)
        print("""// Filter bank used to estimate notes

#ifndef FILTERS_H
#define FILTERS_H

#include <stdint.h>
#include <notes.h>

typedef int16_t Sound_t; // 16 bits for sound input
typedef float Filter_t; // Filter values
typedef float Power_t; // Average power of filter

""")
        print('#define F_SAMPLE %d // Sample rate' % Fs)
        print('#define N_FILTER %d // Comb filter order' % N)
        print('#define A_FILTER %s // Poles slightly inside unit circle' % str(a))
        print()
        print('#define COMB_AN %s' % str(a ** N))
        print('#define RES_A1  2')
        print('#define RES_B2  %s' % str(a ** 2))
        print()
        print('#define MIN_NOTE %s' % min)
        print('#define MAX_NOTE %s' % max)
        print('#define R_NOTE MAX_NOTE - MIN_NOTE + 1')
        print("""
#define N_COMB  4    // Number of comb filter output values to store
#define N_RES   4    // Number of resonator filter output values to store
#define N_POWER 128  // Number of power values to store for each note

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
""")
    elif '--coeff-c' in sys.argv:
        try:
            max = sys.argv.pop()
            min = sys.argv.pop()
            a = float(sys.argv.pop())
            N = int(sys.argv.pop())
            Fs = int(sys.argv.pop())
        except:
            print('Usage ./note_gen.py --coeff-c Fs N a min max', file=sys.stderr)
            sys.exit(1)
        print("""// Filter bank used to estimate notes

#include <filters.h>

const float filter_coeff[R_NOTE] = {""")
        print('  ', ',\n   '.join([str(x) for x in gen_coeff(Fs, N, a, min, max)]))
        print('};')
