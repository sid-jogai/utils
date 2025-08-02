#define DefaultLoadFactor	0.75
#define DefaultCap		32

typedef struct Count Count;
struct Count {
	Str	s;
	i64	count;
};

typedef struct Counter Counter;
struct Counter {
	Count	*counts;
	i64	 len;
	i64	 cap;
	float	 load_factor;
};

/* http://www.cse.yorku.ca/~oz/hash.html */
static u64
djb2(Str str)
{
	u64 hash = 5381;
	for (i64 i = 0; i < str.len; i++)
		hash = ((hash << 5) + hash) + str.b[i];
	return hash;
}

static void
dump_counter(Counter *c, char *name)
{
	printf("%s: %lld/%lld slots used (load factor %.02f%%)\n",
	       name, c->len, c->cap, (c->len/(float)c->cap));
	bool lastslotempty = false;
	for (i64 i = 0; i < c->cap; i++) {
		Count *p = &c->counts[i];
		if (!p->s.len) {
			if (!lastslotempty)
				puts("~");
		} else {
			printf("%lld: %.*s: %lld\n", i, (int)p->s.len, p->s.b, p->count);
		}
		lastslotempty = !p->s.len;
	}
}

static Count *
getcount_no_resize(Counter c, Str s)
{
	assert(c.len<c.cap && c.cap);

	i64 i = djb2(s) % c.cap;
	for (; c.counts[i].s.len; i = (i+1) % c.cap)
		if (str_eq(c.counts[i].s, s))
			break;
	return &c.counts[i];
}

static void
resize_counter(Counter *c, i64 newsize) {
	assert(newsize >= c->cap);

	if (c->load_factor < 0.1)
		c->load_factor = DefaultLoadFactor;

	Counter new = *c;
	new.counts = alloc(0, 0, newsize, sizeof *c->counts);
	new.cap = newsize;
	if (!c->counts)
		goto done;

	for (Count *p = c->counts; p < c->counts+c->len; p++)
		if (p->s.len)
			*getcount_no_resize(new, p->s) = *p;
	free(c->counts);
done:
	*c = new;
}

static Count *
getcount(Counter *c, Str s)
{
	if (!c->cap)
		resize_counter(c, DefaultCap);
	else if (!c->len)
		resize_counter(c, c->cap); /* cap was set on init but no allocation yet */
	else if ((c->len/(float)c->cap) >= c->load_factor)
		resize_counter(c, c->cap*2);

	return getcount_no_resize(*c, s);
}
