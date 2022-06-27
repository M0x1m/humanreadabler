CC=cc
CFLAGS=-O3 -pipe -Wall

humanreadabler: src/humanreadabler.c
	$(CC) -o $@ $< $(CFLAGS)
