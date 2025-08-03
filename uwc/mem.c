static void *
alloc(void *ptr, i64 count, i64 new_count, i64 size)
{
	assert(new_count > count);
	i64 m = new_count * size; /* TODO */
	i64 n = count * size;
	void *p = realloc(ptr, m);
	if (!p)
		abort(); /* TODO */
	memset(p+n, 0, m-n);
	return p;
}

typedef struct Arena Arena;
struct Arena {
	u8	*b;
	Arena	*next;
	i64	 len;
	i64	 cap;
};

static Arena 
init_arena(i64 cap) 
{
	Arena a = {0};
	/* static u8 buf[1024*1024*1]; */
	/* a.b = alloc(0, 0, cap, 1); */
	/* a.b = buf; */
	a.cap = cap;
	return a;
}

static void *
arena_alloc(Arena *a, i64 size)
{
	/* printf("aaloc %ld\n", size); */
	i64 left = a->cap - a->len;
	if (size > left) {
		puts("arena alloc");
		abort();
	}
	void *p = a->b + a->len;
	a->len += size;
	return p;
}

