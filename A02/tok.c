#include <stdio.h>
#include "tok.h"

#define TT_FMT "token %10s:"

extern void emit_token(struct tok *tok)
{
	switch(tok->type) {
	case IDENT_TOK: printf(TT_FMT, "IDENT");
			printf(" %32s\n", tok->val.id);
			break;
	case KEYW_TOK: printf(TT_FMT, "KEYW");
		       printf(" %32d\n", tok->val.keyw);
		       break;
	case CONST_TOK: printf(TT_FMT, "CONST");
			switch (tok->val.cnst.type.type) {
			case I_CONST: printf(" %32" TARGlu "\n",
					     (tulong)tok->val.cnst.val.int_val.ulong_val);
				      break;
			case CH_CONST: printf(" '%c'\n",
					      tok->val.cnst.val.char_val);
				       break;
			default: fprintf(stderr, "error const");
			break;
			}
			break;
	case OP_TOK: printf(TT_FMT, "OP");
		     printf(" %32d\n", tok->val.op);
		     break;
	case DELIM_TOK: printf(TT_FMT, "DELIM");
			printf(" %32d\n", tok->val.delim);
			break;
	default: fprintf(stderr, "Internal error");
	}
}
