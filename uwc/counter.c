#define DefaultLoadFactor	0.7
#define DefaultMinCap		4

typedef struct Count Count;
struct Count
{
	Str	s;
	i64	n;
};

typedef struct Counter Counter;
struct Counter
{
	Count	*counts;
	i64	 len;
	i64	 cap;
	/* for debugging */
	i64 collisions;
};

/* for debugging */
static void
dump_counter(Counter *c, bool verbose)
{
	if (!verbose)
		goto showstats;

	bool lastslotempty = false;
	for (i64 i = 0; i < c->cap; i++) {
		Count *p = &c->counts[i];
		if (!p->s.len) {
			if (!lastslotempty)
				puts("~");
		} else {
			printf("%lld: %.*s: %lld\n", i, (int)p->s.len, p->s.b, p->n);
		}
		lastslotempty = !p->s.len;
	}

showstats:
	printf("%lld/%lld slots occupied; load factor %.02f%%; %lld collisions (%.02f per item)\n",
	       c->len, c->cap, c->len/(float)c->cap, c->collisions, c->collisions/(float)c->len);

}

static Count *
getcount_no_resize(Counter *c, Str s)
{
	assert(c->len<c->cap && c->cap);

	u64 i = hash(s);
	i = i & (c->cap-1);
	while (c->counts[i].s.len) {
		if (str_eq(c->counts[i].s, s)) {
			break;
		}
		i = (i+1) & (c->cap-1);
		c->collisions++;
	}
	return &c->counts[i];
}

static void
resize_counter(Counter *c, i64 newsize)
{
	assert(newsize >= c->cap);

	Counter new = *c;
	new.counts = alloc(0, 0, newsize, sizeof *c->counts);
	new.cap = newsize;
	new.collisions = 0;
	if (!c->counts)
		goto done;

	for (Count *p = c->counts; p < c->counts+c->cap; p++)
		if (p->s.len)
			*getcount_no_resize(&new, p->s) = *p;
	free(c->counts);
done:
	*c = new;
}

static Count *
getcount(Counter *c, Str s)
{
	if (!c->cap)
		resize_counter(c, DefaultMinCap);
	else if ((c->len/(float)c->cap) >= DefaultLoadFactor)
		resize_counter(c, c->cap*2);

	return getcount_no_resize(c, s);
}
