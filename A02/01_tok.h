#ifndef __TOK_H__
#define __TOK_H__

#include "03_ident.h"
#include "04_kword.h"
#include "05_const.h"
#include "op.h"
#include "delim.h"

enum tok_type { IDENT_TOK, KEYW_TOK, CONST_TOK, OP_TOK, DELIM_TOK };

union tok_val {
	struct ident ident;
	enum keyw keyw;
	struct cnst cnst;
	enum op op;
	enum delim delim;
} struct tok {
	enum tok_type type;
	union tok_val val;
};

#endif /* __TOK_H__ */
