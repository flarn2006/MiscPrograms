CC="/usr/bin/gcc"
CFLAGS=-O3 -Wall

all: addletters consine

addletters:
	$(CC) $(CFLAGS) addletters.c -o addletters
consine:
	$(CC) $(CFLAGS) consine.c -lncurses -lm -o consine
