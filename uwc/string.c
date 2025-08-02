typedef struct Str Str;
struct Str
{
	u8	*b;
	i64	 len;
};

#define s(literal) (Str){(u8 *)literal, arraysize(literal)-1}

static bool
str_eq(Str x, Str y)
{
	return x.len==y.len && !memcmp(x.b, y.b, x.len);
}
