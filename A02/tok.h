#ifndef __TOK_H__
#define __TOK_H__

#include "ident.h"
#include "keyw.h"
#include "const.h"
#include "op.h"
#include "delim.h"

enum tok_type { IDENT_TOK, KEYW_TOK, CONST_TOK, OP_TOK, DELIM_TOK };

union tok_val {
	ident id;
	enum keyw keyw;
	struct cnst cnst;
	enum op op;
	enum delim delim;
	int print;
};

struct tok {
	enum tok_type type;
	union tok_val val;
};

extern void emit_token(struct tok *);

#endif /* __TOK_H__ */
