#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "defs.h"
#include "mem.c"
#include "string.c"
#include "hash.c"
#include "counter.c"
#include "os.c"

static bool
whitespace(u8 c)
{
	return c==' ' || c=='\n' || c=='\t';
}

static Counter
count_unique_words(Str buf)
{
	Counter counter = {0};

	bool in_word = false;
	for (u8 *p = buf.b; p < buf.b+buf.len;) {
		if (!in_word) {
			while (p<buf.b+buf.len && whitespace(*p))
				p++;
			in_word = true;
			continue;
		}

		u8 *word = p;
		while (p<buf.b+buf.len && !whitespace(*p))
			p++;
		in_word = false;
		Str s = {word, p-word};

		Count *c = getcount(&counter, s);
		if (!c->s.len) {
			c->s = s;
			counter.len++;
		}
		c->n++;
	}

	return counter;
}

static int
compare_count(const void *x, const void *y)
{
	return ((Count *)y)->n - ((Count *)x)->n;
}

static void
report(Counter c, i64 n)
{
	qsort(c.counts, c.cap, sizeof *c.counts, compare_count);

	if (n <= 0)
		n = INT64_MAX;

	i64 i = 0;
	for (Count *p = c.counts; p<c.counts+c.len && p->s.len && i<n; p++, i++)
		printf("%.*s: %lld\n", (int)p->s.len, p->s.b, p->n);
}

int
main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("usage: %s <file> [max-entries]\n", argv[0]);
		return 0;
	}

	i64 max_entries = 10;
	if (argc == 3) {
		i64 x = strtol(argv[2], 0, 10);
		if (x > 0)
		    max_entries = x;
	}

	char *path = argv[1];
	MemMap m;
	if (!memmap_readonly(path, &m))
		return EXIT_FAILURE;

	Counter counter = count_unique_words(m.s);

	report(counter, max_entries);

	m.unmap(&m);
}
