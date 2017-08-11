CC=gcc
CFLAGS=-Wall -Werror -std=c99

note_tracker : main.o notes.o coeff.o filters.o
	$(CC) -o note_tracker main.o notes.o coeff.o filters.o

clean: 
	rm -f *.o