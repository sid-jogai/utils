void *
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

