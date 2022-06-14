CC=cc
CFLAGS=-O3 -pipe -Wall

humanreadabler:
	$(CC) -o $@ src/humanreadabler.c $(CFLAGS)
