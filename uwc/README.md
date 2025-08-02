A program to count the frequencies of unique words, written to test
out hash tables.

Requirements: C11, POSIX.

To build:

	make

To download some test data (around 7GB):

	make testdata

Usage:

	# print the 20 most frequently used words in the bible (if no
	# number is provied then the top 10 entries are printed)
	./uwc testdata/bible.txt 20

To run the benchmarks (requires https://github.com/sharkdp/hyperfine):

	make benchmark