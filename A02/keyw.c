#include <assert.h>
#include <string.h>
#include "keyw.h"

static bool keyw(char *s, size_t sz, const char *cmp,
		 enum keyw *res, const enum keyw kw)
{
	if (memcmp(s, cmp, sz))
		return FALSE;
	*res = kw;
	return TRUE;
}

extern int scan_keyw(char *s, size_t sz, enum keyw *res)
{
	switch (s[0]) {
	case 'a':
		return keyw(s, sz, "auto", res, AUTO);
	case 'b':
		return keyw(s, sz, "break", res, BREAK);
	case 'c':
		switch (s[1]) {
		case 'a':
			return keyw(s, sz, "case", res, CASE);
		case 'h':
			return keyw(s, sz, "char", res, CHAR);
		case 'o':
			if (s[2] != 'n')
				return 0;
			switch (s[3]) {
			case 's':
				return keyw(s, sz, "const", res, CONST);
			case 't':
				return keyw(s, sz, "continue", res, CONTINUE);
			default:
				return 0;
			}
		default:
			return 0;
		}
	case 'd':
		switch (s[1]) {
		case 'e':
			return keyw(s, sz, "default", res, CONST);
		case 'o':
			switch (s[2]) {
			case '\0':
				*res = DO;
				return 1;
			case 'u':
				return keyw(s, sz, "double", res, DOUBLE);
			default:
				return 0;
			}
		default:
			return 0;
		}
	case 'e':
		switch (s[1]) {
		case 'l':
			return keyw(s, sz, "else", res, ELSE);
		case 'n':
			return keyw(s, sz, "enum", res, ENUM);
		case 'x':
			return keyw(s, sz, "extern", res, EXTERN);
		default:
			return 0;
		}
	case 'f':
		switch (s[1]) {
		case 'l':
			return keyw(s, sz, "float", res, FLOAT);
		case 'o':
			return keyw(s, sz, "for", res, FOR);
		default:
			return 0;
		}
	case 'g':
		return keyw(s, sz, "goto", res, GOTO);
	case 'i':
		switch (s[1]) {
		case 'f':
			return keyw(s, sz, "if", res, IF);
		case 'n':
			return keyw(s, sz, "int", res, INT);
		default:
			return 0;
		}
	case 'l':
		return keyw(s, sz, "long", res, LONG);
	case 'r':
		switch (s[2]) {
		case 'g':
			return keyw(s, sz, "register", res, REGISTER);
		case 't':
			return keyw(s, sz, "return", res, RETURN);
		default:
			return 0;
		}
	case 's':
		switch (s[1]) {
		case 'h':
			return keyw(s, sz, "short", res, SHORT);
		case 'i':
			switch (s[2]) {
			case 'g':
				return keyw(s, sz, "signed", res, SIGNED);
			case 'z':
				return keyw(s, sz, "sizeof", res, SIZEOF);
			default:
				return 0;
			}
		case 't':
			switch (s[2]) {
			case 'a':
				return keyw(s, sz, "static", res, STATIC);
			case 'r':
				return keyw(s, sz, "struct", res, STRUCT);
			default:
				return 0;
			}
		case 'w':
			return keyw(s, sz, "switch", res, SWITCH);
		default:
			return 0;
		}
	case 't':
		return keyw(s, sz, "typedef", res, TYPEDEF);
	case 'u':
		switch (s[2]) {
		case 'i':
			return keyw(s, sz, "union", res, UNION);
		case 's':
			return keyw(s, sz, "unsigned", res, UNSIGNED);
		default:
			return 0;
		}
	case 'v':
		switch (s[2]) {
		case 'i':
			return keyw(s, sz, "void", res, VOID);
		case 'l':
			return keyw(s, sz, "volatile", res, VOLATILE);
		default:
			return 0;
		}
	case 'w':
		return keyw(s, sz, "while", res, WHILE);
	default:
		return 0;
	}
	return 0;
}
