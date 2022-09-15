CC=gcc
CFLAGS=-Wall -Wextra -Werror -Wpedantic -Wshadow -Wwrite-strings \
       -Wstrict-prototypes -Wold-style-definition -Wredundant-decls \
       -Wlogical-op -lm -lSDL2 -lSDL2main

all : main.c
	$(CC) $(CFLAGS) $<

clean : a.out
	rm a.out
