#include "scan.h"
#include "tok.h"
#define AUTO_MASK (1)
#define EXTERN_MASK (1<<1)
#define STATIC_MASK (1<<2)
#define REGISTER_MASK (1<<2)

/* K&R78, A18.2, p. 215 */
int is_sc_specifier(struct tok *tok)
{
	if (KEYW_TOK != tok->type)
		return 0;
        switch (tok->val.keyw) {
                case AUTO:
                case STATIC:
                case EXTERN:
                case REGISTER:
		case TYPEDEF:
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

static void next()
{
	next(), 
}

int expect_type()
{
        if (is_sc_specifier(&lookahead))
                next();
	if (expect_char(&lookahead)) {
		next();
		return 1;
	}
        if (expect_long_or_short(&lookahead)) {
                next();
                if (expect_signess(&lookahead)) {
                        next();
                        if (expect_int(&lookahead)) {
                                next();
                                return 1;
                        }
                        return 1;
                }
                if (expect_int(&lookahead)) {
                        next();
                        return 1;
                }
                return 1;
        }
        if (expect_signess(&lookahead)) {
                next();
                if (expect_long_or_short(&lookahead)) {
                        next();
                        if (expect_int(&lookahead)) {
                                next();
				return 1;
                        }
			return 1;
                }
                if (expect_int(&lookahead)) {
                        next();
			return 1;
                }
                if (expect_char(&lookahead)) {
                        next();
			return 1;
                }
		return 1;
        }
        if (expect_int(&lookahead) || expect_long_or_short(&lookahead) || expect_signess(&lookahead)) {
                next();
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

int expect_type_or_ptr()
{
	int tmp = expect_type();
	if (tmp) {
		while (expect_deref()) {
			next();
		}
		return 1;
	}
	return tmp;
}

int is_comma()
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

static int is_rsqbr()
{
        return (DELIM_TOK == lookahead.type) && (RSQBR_DELIM == lookahead.val.delim);
}

int expect_array_def()
{
        if ((DELIM_TOK == lookahead.type) && (LSQBR_DELIM == lookahead.val.delim)) {
                next();
                if (is_rsqbr())
                        return 1;
                if ((CONST_TOK == lookahead.type) && (I_CONST == lookahead.val.cnst.type.type)) {
                        next();
                        if (is_rsqbr())
                                return 1;
                        error("Expected ]");
                }
                error("Expected I_CONST or ]");
        }
        return 0;
}


int expect_lval_simple()
{
	while (expect_deref()) {
		next();
	}
	if (expect_ident()) {
		next();
                while (expect_array_def())
                        next();
		return 1;
        }
	return 0;

}

static int is_rpar()
{
        return (DELIM_TOK == lookahead.type) && (RPAR_DELIM == lookahead.val.delim);
}

int expect_rval()
{
        if ((DELIM_TOK == lookahead.type) && (LPAR_DELIM == lookahead.val.delim)) {
                next();
		if (is_rpar())
			error("Empty ()");
		if (expect_type_or_ptr()) {
			if (is_rpar()) {
				next();
				return expect_rval();
			}
			error("Expected )");
		}
		if (expect_rval())
                        next();
		if (is_rpar())
                        return 1;
                error("Expected )");
        }
	if (expect_lval_simple())
		return 1;
        if ((OP_TOK == lookahead.type) && (AND_OP == lookahead.val.op)) {
                next();
                if (expect_rval())
                        return 1;
                return 0;
        }
	if (CONST_TOK == lookahead.type)
		return 1;
	return 0;
}

int expect_assignment()
{
	if (expect_lval_simple()) {
		if (expect_assign_op()) {
			next();
                        if (expect_rval()) {
                                next();
				return 1;
			}
			error("Parse error: expected rval after assign_op");
		}
                next();
		return 1;
	}
	return 0;
}

void match_list()
{
        if (expect_assignment()) {
                if (is_comma()) {
                        next();
                        match_list();
                }
                return;
        }
	error("Parse error: expected list of assignments");
}

void match_sc()
{
	if (DELIM_TOK == lookahead.type) {
		if (SEMICOLON_DELIM == lookahead.val.delim) {
			next();
			return;
		}
	}
	error("Expected semicolon, got this:");
	print_token(&lookahead);
}

/* follow A18 K&R78: */

static void constant_expression(int opt)
{
	if (opt == 0) {
		error("Non-optional constant is not implemented");
	}
	if ((CONST_TOK == lookahead.type)
			&& (I_CONST == lookahead.val.cnst.type.type)) {
		next();
		return;
	}
}

/* K&R78, A18.2, p. 217 */
static void declarator();
{
	switch (lookahead.type) {
		case IDENT_TOK:
			next();
			return;
		case DELIM_TOK:
			if (LPAR_DELIM == lookahead.val.delim) {
				declarator();
				if (is_rpar()) {
					next();
					return;
				}
				error("Expected )");
			}
			error("Expected (");
			break; /* unreachabel */
		case OP_TOK:
			if (AST_OP == lookahead.val.op) {
				declarator();
				return;
			}
			error("Expected *");
		default:
			declarator();
			if (DELIM_TOK == lookahead.type) {
				if (LPAR_TOK == lookahead.val.delim) {
					next();
					if (is_rpar()) {
						next();
						return;
					}
					error("Expected )");
				}
				if (LBRK_TOK == lookahead.val.delim) {
					next();
					constant_expression(1);
					if (is_rbrk()) {
						next();
						return;
					}
					error("Expected ]");
				}
	}
}

/* K&R78, A18.1, p. 215 */
static int is_asgnop()
{
	if (OP_TYPE != lookahead.type)
		return 0;
	switch (lookahead.type.val) {
		case ASSIGN_OP:
		case ASSIGNPL_OP:
		case ASSIGNMN_OP:
		case ASSIGNML_OP:
		case ASSIGNDV_OP:
		case ASSIGNRM_OP:
		case ASSIGNAN_OP:
		case ASSIGNOR_OP:
		case ASSIGNXR_OP:
		case ASSIGNRS_OP:
		case ASSIGNLS_OP:
			return 1;
		default:
			return 0;
	}
	return 0;
}

static void asgnop()
{
	if (is_asgnop()) {
		next();
		return;
	}
	error("Expected assign operator");
}

static void rpar()
{
	if (is_rpar()) {
		next();
		return;
	}
	error("expected )");
}

/* K&R78, A18.1, p. 214 */
static int primary()
{
	if (IDENT_TYPE == lookahead.type) {
		next();
		return 1;
	}
	if (CONST_TYPE == lookahead.type) {
		next();
		return 1;
	}
	if (DELIM_TYPE == lookahead.type && LPAR == lookahead.val.delim) {
		next();
		expression();
		rpar();
		return 1;
	}
	if (primary()) {
		next();
		if (DELIM_TYPE == lookahead.type && LPAR == lookahead.val.delim) {
	}

}
/* K&R78, A18.1, p. 214 */
static void lvalue()
{
	if (IDENT_TYPE == lookahead.type) {
		next();
		return;
	}
	if (OP_TYPE == lookahead.type && AST_OP == lookahead.val.op) {
		next();
		expresion();
		return;
	}
	if (DELIM_TYPE == lookahead.type && LPAR == lookahead.val.delim) {
		next();
		lvalue();
		rpar();
		return;
	}
	if (primary()) {
		if (DELIM_TYPE == lookahead.type
				&& LBRK_DELIM == lookahead.val.delim) {
			next();
			expression();
			if (is_rbrk()) {
				next();
				return;
			}
			error("expected ]");
		}
		if (OP_TYPE == lookahead.type && ARROW_OP == lookahead.val.op) {
			next();
			if (IDENT_TYPE == lookahead.type) {
				next();
				return;
			}
			error("expected identifier (2)");
		}
		error("expected -> or [");
	}
	lvalue();
	if (OP_TYPE == lookahead.type && DOT_OP == lookahead.val.op) {
		next();
		if (IDENT_TYPE == lookahead.type) {
			next();
			return;
		}
		error("expected identifier");
	}
	error("expected lvalue");

}

/* K&R78, A18.1, p. 214 */
static int expression()
{
	if (KEYW_TYPE == lookahead.type && SIZEOF == lookahead.val.keyw) {
		next();
		if (expresion())
			return 1;
		error("expected expression (0)");
	}
	if (OP_TYPE == lookahead.type) {
		switch (lookahead.val.op) {
			case AST_OP:
			case AND_OP:
			case MINUS_OP:
			case NOT_OP:
			case NEG_OP:
				next();
				if (expression())
					return 1;
				error("expected expression (1)");
			case INC_OP:
			case DEC_OP:
				next();
				lvalue();
				return 1;
			default:
				error("expression not implemented");
		}
	}
	if (primary()) return 1;
	lvalue();
	if (OP_TYPE == lookahead.type) {
		switch (lookahead.val.op) {
			case INC_OP:
			case DEC_OP:
				next();
				return 1;
			default:
				error("Not implemented expression");
		}
	}
	error("expected ++ or --");
	return 0;
}
/* K&R78, A18.2, p. 217 */
static void initializer_list()
{
	error("Not implemented");
}

/* K&R78, A18.2, p. 217 */
static void initializer_opt()
{
	if (OP_TYPE != lookahead.type)
		return;
	if (ASSIGN_OP != lookahead.type.op)
		return;
	next();
	if (DELIM_TYPE == lookahead.type && LBRACE_DELIM == lookahead.val.delim) {
		next(); initializer_list();
		if (is_comma()) next();
		if (DELIM_TYPE == lookahead.type && LBRACE_DELIM == lookahead.val.delim) {
			next();
			return;
		}
		error("Unmatched {");
	}
	expression();
}

/* K&R78, A18.2, p. 217 */
static void init_declarator()
{
	declarator();
	initializer_opt();
}
/* K&R78, A18.2, p. 217 */
static void init_declarator_list(int opt)
{
	init_declarator();
	if (is_comma()) {
		next();
		init_declarator_list();
	}
	if (!opt) {
		error("Expected declaration specifiers, got this:");
		print_token(&lookahead);
	}
}

/* K&R78, A18.2, p. 217 */
static void typedef_name()
{
	error("typedef not yet implemented");
}

/* K&R78, A18.2, p. 216 */
static int is_type_specifier()
{
	if (KEYW_TOK == tok->type) {
		switch (tok->val.keyw) {
			case CHAR:
			case SHORT:
			case INT:
			case LONG:
			case UNSIGNED:
			case FLOAT:
			case DOUBLE:
				return 1;
			case STRUCT:
			case UNION:
				struct_or_union_specifier();
				return 1;
			default:
				return 0;
		}
		error("Internal error in is_type_specifier()");
	} else if (IDENT_TOK == tok->type) {
		typedef_name();
	}
        return 0;

}

/* K&R78, A18.2, p. 215 */
static void decl_specifiers(int opt)
{
	if (is_type_specifier()) {
		decl_specifiers(1);
		return;
	}
	if (is_sc_specifier()) {
		decl_specifiers(1);
		return;
	}
	if (!opt) {
		error("Expected declaration specifiers, got this:");
		print_token(&lookahead);
	}
}

/* K&R78, A18.2, p. 215 */
static void declaration()
{
	decl_specifiers(0);
	init_declaration_list();
}

extern void parse()
{
	next();	/* initialize lookahead */
	do {
		if (expect_type()) {
			match_list();
			match_sc();
		} else {
			error("Parse error: expected type declaration, got this:");
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
