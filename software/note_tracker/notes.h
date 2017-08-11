// Definition of each note

#ifndef NOTES_H
#define NOTES_H

#include <stdint.h>

// Define each note as its midi number
typedef uint8_t Note_t;

#define N_NOTES 128 // Number of defined notes

#define Cn1    0
#define CSn1   1
#define Dn1    2
#define Ebn1   3
#define En1    4
#define Fn1    5
#define FSn1   6
#define Gn1    7
#define GSn1   8
#define An1    9
#define Bbn1   10
#define Bn1    11
#define C0     12
#define CS0    13
#define D0     14
#define Eb0    15
#define E0     16
#define F0     17
#define FS0    18
#define G0     19
#define GS0    20
#define A0     21
#define Bb0    22
#define B0     23
#define C1     24
#define CS1    25
#define D1     26
#define Eb1    27
#define E1     28
#define F1     29
#define FS1    30
#define G1     31
#define GS1    32
#define A1     33
#define Bb1    34
#define B1     35
#define C2     36
#define CS2    37
#define D2     38
#define Eb2    39
#define E2     40
#define F2     41
#define FS2    42
#define G2     43
#define GS2    44
#define A2     45
#define Bb2    46
#define B2     47
#define C3     48
#define CS3    49
#define D3     50
#define Eb3    51
#define E3     52
#define F3     53
#define FS3    54
#define G3     55
#define GS3    56
#define A3     57
#define Bb3    58
#define B3     59
#define C4     60
#define CS4    61
#define D4     62
#define Eb4    63
#define E4     64
#define F4     65
#define FS4    66
#define G4     67
#define GS4    68
#define A4     69
#define Bb4    70
#define B4     71
#define C5     72
#define CS5    73
#define D5     74
#define Eb5    75
#define E5     76
#define F5     77
#define FS5    78
#define G5     79
#define GS5    80
#define A5     81
#define Bb5    82
#define B5     83
#define C6     84
#define CS6    85
#define D6     86
#define Eb6    87
#define E6     88
#define F6     89
#define FS6    90
#define G6     91
#define GS6    92
#define A6     93
#define Bb6    94
#define B6     95
#define C7     96
#define CS7    97
#define D7     98
#define Eb7    99
#define E7     100
#define F7     101
#define FS7    102
#define G7     103
#define GS7    104
#define A7     105
#define Bb7    106
#define B7     107
#define C8     108
#define CS8    109
#define D8     110
#define Eb8    111
#define E8     112
#define F8     113
#define FS8    114
#define G8     115
#define GS8    116
#define A8     117
#define Bb8    118
#define B8     119
#define C9     120
#define CS9    121
#define D9     122
#define Eb9    123
#define E9     124
#define F9     125
#define FS9    126
#define G9     127

// Cn1 is below the frequency range of what we can actually measure
// Use this to indicate no note
#define NO_NOTE 0

extern const char * note_names[N_NOTES];
extern const float note_freq[N_NOTES];

#endif

