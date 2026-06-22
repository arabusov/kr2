#include <stdio.h>
#include "tok.h"

#define TT_FMT "token %10s:"

extern void emit_token(struct tok *tok)
{
	switch (tok->type) {
	case IDENT_TOK:
		printf(TT_FMT, "IDENT");
		printf(" %32s\n", tok->val.id);
		break;
	case KEYW_TOK:
		printf(TT_FMT, "KEYW");
		printf(" %32d\n", tok->val.keyw);
		break;
	case CONST_TOK:
		printf(TT_FMT, "CONST");
		switch (tok->val.cnst.type.type) {
		case I_CONST:
			switch (tok->val.cnst.type.int_type) {
				case ULONG_CONST: printf(" %31" TARGlu "h\n",
			       (tulong) tok->val.cnst.val.int_val.ulong_val);
						  break;
				case LONG_CONST: printf(" %32" TARGld "\n",
			       (tlong) tok->val.cnst.val.int_val.long_val);
						  break;
				case UINT_CONST: printf(" %31" TARGu "h\n",
			       (tuint) tok->val.cnst.val.int_val.uint_val);
						  break;
				case INT_CONST: printf(" %32" TARGd "\n",
			       (tint) tok->val.cnst.val.int_val.int_val);
						  break;
				default: printf(" unknown const type \n");
					 break;
			}
			break;
		case CH_CONST:
			printf(" '%32c'\n", tok->val.cnst.val.char_val);
			break;
		case STR_CONST:
			printf(" '%32s'\n", tok->val.cnst.val.str_val);
			break;
		default:
			fprintf(stderr, "error const");
			break;
		}
		break;
	case OP_TOK:
		printf(TT_FMT, "OP");
		printf(" %32d\n", tok->val.op);
		break;
	case DELIM_TOK:
		printf(TT_FMT, "DELIM");
		printf(" %32d\n", tok->val.delim);
		break;
	case EOF_TOK:
		break;
	case INVALID_TOK:
		printf(TT_FMT, "INVAL");
		printf("\n");
		break;
	default:
		fprintf(stderr, "emit_token: Internal error: %d\n", tok->type);
		break;
	}
}
