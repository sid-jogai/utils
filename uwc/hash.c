u64
fnv1a_64(Str s)
{
	u64 v = 14695981039346656037U;
	for (i64 i = 0; i < s.len; i++) {
		v ^= s.b[i];
		v *= 1099511628211;
	}
	return v;
}

u64 
hash(Str s)
{
	return fnv1a_64(s);
}
