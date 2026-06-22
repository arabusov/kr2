#ifndef __KEYW_H__
#define __KEYW_H__

#include <stddef.h>
#include "bool.h"

enum keyw {
	NOT_KEYW,
	AUTO, DOUBLE, INT, STRUCT,
	BREAK, ELSE, LONG, SWITCH,
	CASE, ENUM, REGISTER, TYPEDEF,
	CHAR, EXTERN, RETURN, UNION,
	CONST, FLOAT, SHORT, UNSIGNED,
	CONTINUE, FOR, SIGNED, VOID,
	DEFAULT, GOTO, SIZEOF, VOLATILE,
	DO, IF, STATIC, WHILE
};

extern bool scan_keyw(char *, size_t, enum keyw *);

#endif /* __KEYW_H__ */
