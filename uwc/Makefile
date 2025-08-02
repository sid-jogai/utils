CC=cc
DEBUG_FLAGS=-Wall -Wextra -fsanitize=address,undefined -g3
CFLAGS=-Wall -Wextra -O3

all:
	$(CC) $(CFLAGS) uwc.c -o uwc

debug:
	$(CC) $(DEBUG_FLAGS) uwc.c -o uwc_debug

clean:
	rm -rf uwc uwc_debug uwc_debug.dSYM

.PHONY: all debug
