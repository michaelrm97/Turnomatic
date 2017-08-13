#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filters.h"

#define MAXLEN 16
#define LOUD_NOTES 4

int main(int argc, char **argv) {

    if (argc < 2) {
        fprintf(stderr, "Usage: %s file [max_it]\n", argv[0]);
        return EXIT_FAILURE;
    }
    FILE *stream = fopen(argv[1], "r");
    if (stream == NULL) {
        fprintf(stderr, "Error: Cannot open file: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    int i;
    #if 0
    Note_t note;
    for (i = 0; i < N_NOTES; i++) {
        if (!strcmp(argv[2], note_names[i])) {
            note = i;
            break;
        }
    }
    if (i == N_NOTES) {
        fprintf(stderr, "Usage: %s file note [max_it]\n", argv[0]);
        return EXIT_FAILURE;
    }
    printf("%d\n", note);
    #endif

    int max_it;
    if (argc > 2) {
        max_it = atoi(argv[2]);
    } else {
        max_it = -1;
    }

    filter_reset();

    i = 0;
    char line[MAXLEN];

    while (fgets(line, MAXLEN, stream) && (i < max_it || max_it == -1)) {
        Note_t loudest_notes[LOUD_NOTES];
        Power_t loudest_power[LOUD_NOTES];
        
        filter_input(atoi(line));
        uint8_t n_loud = filter_notes(loudest_notes, loudest_power, LOUD_NOTES, 1e12);

        printf("%d: ", i);
        for (int j = 0; j < n_loud; j++) {
            printf("%s- %.2f ", note_names[loudest_notes[j]], loudest_power[j] / 1e12);
        }
        printf("\n");
        i++;
    }
    fclose(stream);
    return EXIT_SUCCESS;
}
