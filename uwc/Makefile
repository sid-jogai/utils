CC=cc
DEBUG_FLAGS=-Wall -Wextra -fsanitize=address,undefined -g3
CFLAGS=-Wall -Wextra -O3

all:
	$(CC) $(CFLAGS) main.c -o uwc

debug:
	$(CC) $(DEBUG_FLAGS) main.c -o uwc_debug

clean:
	rm -rf uwc uwc_debug uwc_debug.dSYM

testdata:
	bash gettestdata.sh

benchmark:
	bash benchmark.sh

.PHONY: all debug clean testdata benchmark
