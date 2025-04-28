#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "bool.h"
#include "tok.h"

#define COL_LIM 511

static int lineno = 1, col = 1;

extern void error(const char *s)
{
	fprintf(stderr, "Error at %d[L]:%d[C] -- %s\n", lineno, col, s);
	exit(EXIT_FAILURE);
}

static void test_at_eof(void)
{
}

int is_whitespace(char c)
{
	return (c == ' ') || (c == '\t');
}

int is_upper_case(char c)
{
	return (c >= 'A') && (c <= 'Z');
}

int is_lower_case(char c)
{
	return (c >= 'a') && (c <= 'z');
}

int is_letter(char c)
{
	return is_upper_case(c) || is_lower_case(c);
}

int is_digit(char c)
{
	return (c >= '0') && (c <= '9');
}

static int gch(void)
{
	int ch = getchar();
	if (ch == '\n') {
		lineno++;
		col = 1;
		return ch;
	}
	if (ch == EOF) {
		test_at_eof();
		return ch;
	}
	col++;
	if (col > COL_LIM)
		error("Column limit exceeded");
	return ch;
}

static void ungch(int ch)
{
	ungetc(ch, stdin);
	col--;
	if (col < 0)
		error("Column below zero");
}

int expect_alphanum(struct tok *t)
{
	char s[IDENT_LEN + 1];
	size_t sz = 0;
	int c = gch();
	if (!is_letter(c))
		return FALSE;
	do {
		s[sz++] = c;
		c = gch();
		if (sz >= IDENT_LEN) {
			error("Exceeded alphanum capasity");
		}
	} while (is_digit(s[sz]) || is_letter(s[sz]));
	ungch(c);
	/* Now, string s[] is ready */
	if (scan_iconst(s, sz, &t->val.cnst)) {
		t->type = CONST_TOK;
		return TRUE;
	}
	if (scan_keyw(s, sz, &t->val.keyw)) {
		t->type = KEYW_TOK;
		return TRUE;
	}
	t->type = IDENT_TOK;
	t->val.id = save_ident(s, sz);
	return TRUE;
}

static void ignore_comment(void)
{
	int ch;
	do {
		ch = gch();
		if (ch == '*') {
			ch = gch();
			if (EOF == ch) {
				error("Unexpected EOF while scanning comment");
			}
			if (ch == '/') {
				return;
			}
			ungch(ch);
		}
		ungch(ch);
	} while (EOF != ch);
}

static bool is_comment_start()
{
	int ch = gch();
	if (ch == '/') {
		ch = gch();
		if (EOF == ch) {
			return FALSE;
		}
		if ('*' == ch) {
			return TRUE;
		}
		ungch(ch);
	}
	ungch(ch);
	return FALSE;
}

bool expect(int c)
{
	if (c == gch())
		return TRUE;
	ungch(c);
	return FALSE;
}

bool expect_delim(enum delim *delim)
{
	int ch = gch();
	if (IS_DELIM(ch)) {
		*delim = ch;
		return TRUE;
	}
	ungch(ch);
	return FALSE;
}

#define EXP_NEXT(NEXT, AO)\
	if (expect(NEXT)) { \
		*op = AO; \
		return TRUE; \
	}
extern bool expect_operator(enum op *op)
{
	int ch = gch();
	switch (ch) {
	case '*':
		*op = AST_OP;
		return TRUE;
	case ',':
		*op = COMMA_OP;
		return TRUE;
	case '?':
		*op = QMARK_OP;
		return TRUE;
	case '+':
		EXP_NEXT('=', ASSIGNPL_OP);
		EXP_NEXT('+', INC_OP);
		*op = PLUS_OP;
		return TRUE;
	case '-':
		EXP_NEXT('=', ASSIGNMN_OP);
		EXP_NEXT('-', DEC_OP);
		EXP_NEXT('>', ARROW_OP);
		*op = MINUS_OP;
		return TRUE;
	case '/':
		EXP_NEXT('/', ASSIGNDV_OP);
		*op = DIV_OP;
		return TRUE;
	case '%':
		EXP_NEXT('=', ASSIGNRM_OP);
		*op = REM_OP;
		return TRUE;
	case '~':
		*op = NEG_OP;
		return TRUE;
	case '&':
		EXP_NEXT('=', ASSIGNAN_OP);
		EXP_NEXT('&', LAND_OP);
		*op = AND_OP;
		return TRUE;
	case '|':
		EXP_NEXT('=', ASSIGNOR_OP);
		EXP_NEXT('|', LOR_OP);
		*op = OR_OP;
		return TRUE;
	case '^':
		EXP_NEXT('=', ASSIGNXR_OP);
		*op = XOR_OP;
		return TRUE;
	case '!':
		EXP_NEXT('=', NEQ_OP);
		*op = NOT_OP;
		return TRUE;
	case '=':
		EXP_NEXT('=', EQ_OP);
		*op = ASSIGN_OP;
		return TRUE;
	case '.':
		if (expect('.')) {
			if (expect('.')) {
				*op = ELLI_OP;
				return TRUE;
			}
		}
		*op = DOT_OP;
		return TRUE;
	case '<':
		if (expect('<')) {
			if (expect('=')) {
				*op = ASSIGNLS_OP;
				return TRUE;
			}
			*op = LS_OP;
			return TRUE;
		}
		EXP_NEXT('=', LEQ_OP);
		*op = LESS_OP;
		return TRUE;
	case '>':
		if (expect('>')) {
			if (expect('=')) {
				*op = ASSIGNRS_OP;
				return TRUE;
			}
			*op = RS_OP;
			return TRUE;
		}
		EXP_NEXT('=', GEQ_OP);
		*op = GR_OP;
		return TRUE;
	}
	ungch(ch);
	return FALSE;
}

void skip_wsp(void)
{
	int ch;
	do {
		ch = gch();
	} while (is_whitespace(ch));
	ungch(ch);
}

#define CCHAR_BUF_LEN 32
bool expect_cconst(struct cnst *cnst)
{
	char buf[CCHAR_BUF_LEN];
	size_t sz = 0;
	int ch = gch();
	if ('\'' != ch) {
		ungch(ch);
		return FALSE;
	}
	do {
		buf[sz] = ch;
		ch = gch();
		if (sz >= CCHAR_BUF_LEN) {
			error("Exceeded char buffer capasity");
		}
		sz++;
		if (EOF == ch) {
			error("Unexpected EOF while scanning char const");
		}
	} while (ch != '\'');
	buf[sz++] = ch;
	/* no ungch() at the end, because it will return the closing "'" */
	return scan_cconst(buf, sz, cnst);
}

#define NTOKMAX 2048
struct tok tok_table[NTOKMAX];
int toki = 0;

extern void scan(void)
{
	do {
		struct tok *tokp = &tok_table[toki];
		skip_wsp();
		if (is_comment_start()) {
			ignore_comment();
			continue; /* don't create a new tok record */
		} else if (expect_delim(&(tokp->val.delim))) {
			tokp->type = DELIM_TOK;
		} else if (expect_operator(&(tokp->val.op))) {
			tokp->type = OP_TOK;
		} else if (expect_cconst(&(tokp->val.cnst))) {
			tokp->type = CONST_TOK;
		} else if (expect_alphanum(tokp)) {
		} else if (expect(EOF)) {
			test_at_eof();
		} else {
			error("Unknown symbol");
		}
		/* Now, update the token table */
		if (toki < NTOKMAX) {
			toki++;
		} else {
			error("Token's capasity exceeded");
		}
	}
	while (TRUE);
}

int main(void)
{
	scan();
	return 0;
}
