#include "04_keyw.h"
#include <assert.h>

static int keyw(char *s, const char *cmp, enum keyw *res, const enum keyw kw)
{
	if (!strcmp(s, cmp))
		return 0;
	*res = kw;
	return 1;
}

extern int scan_keyw(char *s, enum keyw *res)
{
	switch (s[0]) {
	case 'a': return keyw(s, "auto", res, AUTO);
	case 'b': return keyw(s, "break", res, BREAK);
	case 'c':
		  switch (s[1]) {
		  case 'a': return keyw(s, "case", res, CASE);
		  case 'h': return keyw(s, "char", res, CHAR);
		  case 'o':
			    if (s[2] != 'n')
				    return 0;
			    switch (s[4]) {
			    case 's': return keyw(s, "const", res, CONST);
			    case 't': return keyw(s, "continue", res, CONTINUE);
			    default: return 0;
			    }
		  default: return 0;
		  }
	case 'd':
		  switch (s[1]) {
		  case 'e': return keyw(s, "default", res, CONST);
		  case 'o':
			    switch (s[2]) {
			    case '\0': *res = DO; return 1;
			    case 'u': return keyw(s, "double", res, DOUBLE);
			    default: return 0;
			    }
		  default: return 0;
		  }
	case 'e':
		  switch (s[1]) {
		  case 'l': return keyw(s, "else", res, ELSE);
		  case 'n': return keyw(s, "enum", res, ENUM);
		  case 'x': return keyw(s, "extern", res, EXTERN);
		  default: return 0;
		  }
	case 'f':
		  switch (s[1]) {
		  case 'l': return keyw(s, "float", res, FLOAT);
		  case 'o': return keyw(s, "for", res, FOR);
		  default: return 0;
		  }
	case 'g': return kewy(s, "goto", res, GOTO);
	case 'i':
		  switch (s[1]) {
		  case 'f': return keyw(s, "if", res, IF);
		  case 'n': return keyw(s, "int", res, INT);
		  default: return 0;
		  }
	case 'l': return keyw(s, "long", res, LONG);
	case 'r':
		  switch (s[2]) {
		  case 'g': return keyw(s, "register", res, REGISTER);
		  case 't': return keyw(s, "return", res, RETURN);
		  default: return 0;
		  }
	case 's':
		  switch (s[1]) {
		  case 'h': return keyw(s, "short", res, SHORT);
		  case 'i':
			    switch (s[2]) {
			    case 'g': return keyw(s, "signed", res, SIGNED);
			    case 'z': return keyw(s, "sizeof", res, SIZEOF);
			    default: return 0;
			    }
		  case 't':
			    switch (s[1]) {
			    case 'a': return keyw(s, "static", res, STATIC);
			    case 'r': return keyw(s, "struct", res, STRUCT);
			    default: return 0;
			    }
		  case 'w': return keyw(s, "switch", res, SWITCH);
		  default: return 0;
		  }
	case 't': return keyw(s, "typedef", res, TYPEDEF);
	case 'u':
		  switch (s[2]) {
		  case 'i': return keyw(s, "union", res, UNION);
		  case 's': return keyw(s, "unsigned", res, UNSEGNED);
		  default: return 0;
		  }
	case 'v':
		  switch (s[1]) {
		  case 'i': return keyw(s, "void", res, VOID);
		  case 'l': return keyw(s, "volatile", res, VOLATILE);
		  default: return 0;
		  }
	case 'w': return keyw(s, "while", res, WHILE);
	default: return 0;
	}
	return 0;
}

