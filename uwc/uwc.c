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
#include "counter.c"
#include "os.c"

static bool
whitespace(u8 c)
{
	return c==' ' || c=='\n' || c=='\t';
}

static Counter
count_unique_words(u8 *buf, i64 len)
{
	Counter c = {0};

	bool in_word = false;
	for (u8 *p = buf; p < buf+len;) {
		if (in_word) {
			u8 *word = p;
			while (p<buf+len && !whitespace(*p))
				p++;
			i64 len = p-word;
			in_word = false;

			Count *kv = getcount(&c, (Str){.b = word, len});
			if (!kv->s.len) {
				Str s = {.b = alloc(0, 0, len, 1), .len = len};
				memcpy(s.b, word, len);
				kv->s = s;
				c.len++;
			}
			kv->count++;
		} else {
			for (; p<buf+len && whitespace(*p); p++)
				;
			in_word = true;
		}
	}

	return c;
}

static int
compare_count(const void *x, const void *y)
{
	return ((Count *)y)->count - ((Count *)x)->count;
}

static void
report(Counter c, i64 n)
{
	qsort(c.counts, c.cap, sizeof *c.counts, compare_count);

	if (n <= 0)
		n = INT64_MAX;

	i64 i = 0;
	for (Count *p = c.counts; p<c.counts+c.len && p->s.len && i<n; p++, i++)
		printf("%.*s: %lld\n", (int)p->s.len, p->s.b, p->count);
}

int
main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("usage: %s <file>\n", argv[0]);
		return 0;
	}

	char *path = argv[1];
	MemMap m;
	if (!memmap_readonly(path, &m)) {
		return 2;
	}

	Counter counter = count_unique_words(m.b, m.len);
	report(counter, 10);
}
