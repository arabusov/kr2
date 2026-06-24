#include "scan.h"
#include "tok.h"
#define AUTO_MASK (1)
#define EXTERN_MASK (1<<1)
#define STATIC_MASK (1<<2)
#define REGISTER_MASK (1<<2)

int expect_modifier(struct tok *tok)
{
	if (KEYW_TOK != tok->type)
		return 0;
        switch (tok->val.keyw) {
                case AUTO:
                case EXTERN:
                case STATIC:
                case REGISTER:
                        return 1;
                default: return 0;
        }
        return 0;
}

int expect_char(struct tok *tok)
{
	if (KEYW_TOK != tok->type)
		return 0;
        return (CHAR == tok->val.keyw);
}

int expect_int(struct tok *tok)
{
	if (KEYW_TOK != tok->type)
		return 0;
        return (INT == tok->val.keyw);
}

int expect_signess(struct tok *tok)
{
	if (KEYW_TOK != tok->type)
		return 0;
        return (UNSIGNED == tok->val.keyw) || (SIGNED == tok->val.keyw);
}

int expect_long_or_short(struct tok *tok)
{
	if (KEYW_TOK != tok->type)
		return 0;
        return (LONG == tok->val.keyw) || (SHORT == tok->val.keyw);
}

struct tok lookahead;

int expect_type()
{
        if (expect_modifier(&lookahead))
                scan(&lookahead);
	if (expect_char(&lookahead)) {
		scan(&lookahead);
		return 1;
	}
        if (expect_long_or_short(&lookahead)) {
                scan(&lookahead);
                if (expect_signess(&lookahead)) {
                        scan(&lookahead);
                        if (expect_int(&lookahead)) {
                                scan(&lookahead);
                                return 1;
                        }
                        return 1;
                }
                if (expect_int(&lookahead)) {
                        scan(&lookahead);
                        return 1;
                }
                return 1;
        }
        if (expect_signess(&lookahead)) {
                scan(&lookahead);
                if (expect_long_or_short(&lookahead)) {
                        scan(&lookahead);
                        if (expect_int(&lookahead)) {
                                scan(&lookahead);
				return 1;
                        }
			return 1;
                }
                if (expect_int(&lookahead)) {
                        scan(&lookahead);
			return 1;
                }
                if (expect_char(&lookahead)) {
                        scan(&lookahead);
			return 1;
                }
		return 1;
        }
        if (expect_int(&lookahead) || expect_long_or_short(&lookahead) || expect_signess(&lookahead)) {
                scan(&lookahead);
		return 1;
        }

	return 0;


}

int expect_deref()
{
	if (OP_TOK == lookahead.type)
		if (AST_OP == lookahead.val.op)
			return 1;
	return 0;
}

int expect_comma()
{
	if (OP_TOK == lookahead.type)
		if (COMMA_OP == lookahead.val.op)
			return 1;
	return 0;
}

int expect_assign_op()
{
	if (OP_TOK == lookahead.type)
		if (ASSIGN_OP == lookahead.val.op)
			return 1;
	return 0;
}

int expect_ident()
{
	if (IDENT_TOK == lookahead.type)
		return 1;
	return 0;
}

int expect_lval_simple()
{
	if (expect_deref())
		scan(&lookahead);
	if (expect_ident())
		return 1;
	error("Parse error: expected identifier");
	return 0;
}

static int is_rpar()
{
        return (DELIM_TOK == lookahead.type) && (RPAR_DELIM == lookahead.val.delim);
}

int expect_rval()
{
        if ((DELIM_TOK == lookahead.type) && (LPAR_DELIM == lookahead.val.delim)) {
                scan(&lookahead);
		if (is_rpar())
			error("Empty ()");
                if (expect_rval())
                        scan(&lookahead);
                if (is_rpar())
                        return 1;
                error("Expected )");
        }
        if (OP_TOK == lookahead.type) {
                if (AND_OP == lookahead.val.op) {
                        scan(&lookahead);
                        if (expect_rval())
                                return 1;
                        return 0;
                }
                return 0;
        }
	return (IDENT_TOK == lookahead.type) || (CONST_TOK == lookahead.type);
}

static int is_rsqbr()
{
        return (DELIM_TOK == lookahead.type) && (RSQBR_DELIM == lookahead.val.delim);
}

int expect_array_def()
{
        if ((DELIM_TOK == lookahead.type) && (LSQBR_DELIM == lookahead.val.delim)) {
                scan(&lookahead);
                if (is_rsqbr())
                        return 1;
                if ((CONST_TOK == lookahead.type) && (I_CONST == lookahead.val.cnst.type.type)) {
                        scan(&lookahead);
                        if (is_rsqbr())
                                return 1;
                        error("Expected ]");
                }
                error("Expected I_CONST or ]");
        }
        return 0;
}

int expect_assignment()
{
	if (expect_lval_simple()) {
		scan(&lookahead);
                while (expect_array_def())
                        scan(&lookahead);
		if (expect_assign_op()) {
			scan(&lookahead);
                        if (expect_rval()) {
                                scan(&lookahead);
				return 1;
			}
			error("Parse error: expected rval after assign_op");
		}
		return 1;
	}
	return 0;
}

void match_list()
{
	if (expect_assignment()) {
		return;
	}
	if (expect_comma()) {
		match_list();
		return;
	}
	error("Parse error: expected list of assignments");
}

void match_sc()
{
	if (DELIM_TOK == lookahead.type) {
		if (SEMICOLON_DELIM == lookahead.val.delim) {
			scan(&lookahead);
			return;
		}
	}
	error("Expected semicolon, got this:");
	print_token(&lookahead);
}

extern void parse()
{
	scan(&lookahead);	/* initialize lookahead */
	do {
		if (expect_type()) {
			match_list();
			match_sc();
		} else {
			error("Parse error: expected basic type, got this:");
			print_token(&lookahead);
		}
	} while ((EOF_TOK != lookahead.type)
			&& (INVALID_TOK != lookahead.type));
}

int main()
{
	parse();
	return 0;
}
