#include <assert.h>
#include <string.h>
#include "keyw.h"

static bool keyw(char *s, size_t sz, const char *cmp, const size_t ksz,
		 enum keyw *res, const enum keyw kw)
{
	if (sz == ksz) {
		if (memcmp(s, cmp, sz))
			return FALSE;
		*res = kw;
		return TRUE;
	}
	return FALSE;
}

#define DM(x) (x), (sizeof(x)-1)

extern int scan_keyw(char *s, size_t sz, enum keyw *res)
{
	switch (s[0]) {
	case 'a':
		return keyw(s, sz, DM("auto"), res, AUTO);
	case 'b':
		return keyw(s, sz, DM("break"), res, BREAK);
	case 'c':
		switch (s[1]) {
		case 'a':
			return keyw(s, sz, DM("case"), res, CASE);
		case 'h':
			return keyw(s, sz, DM("char"), res, CHAR);
		case 'o':
			if (s[2] != 'n')
				return 0;
			switch (s[3]) {
			case 's':
				return keyw(s, sz, DM("const"), res, CONST);
			case 't':
				return keyw(s, sz, DM("continue"), res, CONTINUE);
			default:
				return 0;
			}
		default:
			return 0;
		}
	case 'd':
		switch (s[1]) {
		case 'e':
			return keyw(s, sz, DM("default"), res, CONST);
		case 'o':
			switch (s[2]) {
			case '\0':
				*res = DO;
				return 1;
			case 'u':
				return keyw(s, sz, DM("double"), res, DOUBLE);
			default:
				return 0;
			}
		default:
			return 0;
		}
	case 'e':
		switch (s[1]) {
		case 'l':
			return keyw(s, sz, DM("else"), res, ELSE);
		case 'n':
			return keyw(s, sz, DM("enum"), res, ENUM);
		case 'x':
			return keyw(s, sz, DM("extern"), res, EXTERN);
		default:
			return 0;
		}
	case 'f':
		switch (s[1]) {
		case 'l':
			return keyw(s, sz, DM("float"), res, FLOAT);
		case 'o':
			return keyw(s, sz, DM("for"), res, FOR);
		default:
			return 0;
		}
	case 'g':
		return keyw(s, sz, DM("goto"), res, GOTO);
	case 'i':
		switch (s[1]) {
		case 'f':
			return keyw(s, sz, DM("if"), res, IF);
		case 'n':
			return keyw(s, sz, DM("int"), res, INT);
		default:
			return 0;
		}
	case 'l':
		return keyw(s, sz, DM("long"), res, LONG);
	case 'r':
		switch (s[2]) {
		case 'g':
			return keyw(s, sz, DM("register"), res, REGISTER);
		case 't':
			return keyw(s, sz, DM("return"), res, RETURN);
		default:
			return 0;
		}
	case 's':
		switch (s[1]) {
		case 'h':
			return keyw(s, sz, DM("short"), res, SHORT);
		case 'i':
			switch (s[2]) {
			case 'g':
				return keyw(s, sz, DM("signed"), res, SIGNED);
			case 'z':
				return keyw(s, sz, DM("sizeof"), res, SIZEOF);
			default:
				return 0;
			}
		case 't':
			switch (s[2]) {
			case 'a':
				return keyw(s, sz, DM("static"), res, STATIC);
			case 'r':
				return keyw(s, sz, DM("struct"), res, STRUCT);
			default:
				return 0;
			}
		case 'w':
			return keyw(s, sz, DM("switch"), res, SWITCH);
		default:
			return 0;
		}
	case 't':
		return keyw(s, sz, DM("typedef"), res, TYPEDEF);
	case 'u':
		switch (s[2]) {
		case 'i':
			return keyw(s, sz, DM("union"), res, UNION);
		case 's':
			return keyw(s, sz, DM("unsigned"), res, UNSIGNED);
		default:
			return 0;
		}
	case 'v':
		switch (s[2]) {
		case 'i':
			return keyw(s, sz, DM("void"), res, VOID);
		case 'l':
			return keyw(s, sz, DM("volatile"), res, VOLATILE);
		default:
			return 0;
		}
	case 'w':
		return keyw(s, sz, DM("while"), res, WHILE);
	default:
		return 0;
	}
	return 0;
}
