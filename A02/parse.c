#include "scan.h"
#include "tok.h"

int expect_basic_type(struct tok *tok)
{
	if (KEYW_TOK != tok->type)
		return 0;
	switch (tok->val.keyw) {
		case CHAR:
		case INT:
		case UNSIGNED:
		case LONG:
			return 1;
		default: return 0;
	}
	return 0;
}

struct tok lookahead;

void match_type()
{
	if (expect_basic_type(&lookahead)) {
		scan(&lookahead);
		return;
	}
	error("Parse error: expected basic type, got this:");
	emit_token(&lookahead);
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

int expect_lval()
{
	if (expect_deref())
		scan(&lookahead);
	if (expect_ident())
		return 1;
	error("Parse error: expected identifier");
	return 0;
}

int expect_rval()
{
	return (IDENT_TOK == lookahead.type) || (CONST_TOK == lookahead.type);
}

int expect_assignment()
{
	if (expect_lval()) {
		scan(&lookahead);
		if (expect_assign_op()) {
			scan(&lookahead);
			if (expect_rval()) {
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
		scan(&lookahead);
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
	emit_token(&lookahead);
}

extern void parse()
{
	scan(&lookahead);	/* initialize lookahead */
	emit_token(&lookahead);
	do {
		match_type(); match_list(); match_sc();
	} while ((EOF_TOK != lookahead.type)
			&& (INVALID_TOK != lookahead.type));
}

int main()
{
	parse();
	return 0;
}
