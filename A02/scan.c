#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "bool.h"
#include "tok.h"

#define COL_LIM 511
static char curr_str[COL_LIM+1];
static int lineno = 1, col = 1, prev_line_col = 1;

#define FNAME_LIM 256
static char fname[FNAME_LIM];

#define N_LM_FLAGS 4
static bool lm_flags[N_LM_FLAGS];

extern void error(const char *s)
{
	fprintf(stderr, "Error at %d[L]:%d[C] -- %s\n", lineno, col, s);
	fprintf(stderr, "%s", curr_str);
	scanf("%s", curr_str);
	fprintf(stderr, "%s\n", curr_str);
	exit(EXIT_FAILURE);
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

static int is_letter(char c)
{
	return is_upper_case(c) || is_lower_case(c);
}

static bool is_c_letter(char c)
{
	return is_letter(c) || ('_' == c);
}

static int gch(void)
{
	int ch;
	ch = getchar();
	if ('\n' == ch) {
		lineno++;
		prev_line_col = col;
		col = 1;
		memset(curr_str, 0, COL_LIM);
		return ch;
	}
	if (EOF == ch)
		return EOF;
	curr_str[col-1] = ch;
	col++;
	if (col > COL_LIM)
		error("Column limit exceeded");
	return ch;
}

static void ungch(int ch)
{
	if (EOF == ch)
		return;
	ungetc(ch, stdin);
	if ('\n' != ch) {
		col--;
		if (col < 0)
			error("Column number below zero");
	} else {
		lineno--;
		col = prev_line_col;
		if (lineno < 0)
			error("Line number below zero");
	}
}

int expect_alphanum(struct tok *t)
{
	char s[IDENT_LEN + 1];
	size_t sz = 0;
	int c;
	do {
		c = gch();
		s[sz++] = c;
		if (sz >= IDENT_LEN) {
			error("Exceeded alphanum capasity");
		}
	} while (is_digit(c) || is_c_letter(c));
	ungch(c);
	sz--;
	if (sz == 0)
		return FALSE;
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
	int sym = gch();
	if (c == sym)
		return TRUE;
	ungch(sym);
	return FALSE;
}

static void ignore_comment(void)
{
	int ch;
	do {
		if (expect('*'))
			if (expect('/'))
				return;
		ch = gch();
		if (EOF == ch)
			error("Unexpected EOF while scanning comment");
	} while (EOF != ch);
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

static bool linemarkers(void)
{
	/* # 474 "/usr/include/features.h" 2 3 4 */
	/* hash and space are eaten already */
	int i, lfl[4];
	int res = scanf("%d %s %d %d %d %d", &lineno, fname,
			&lfl[0], &lfl[1], &lfl[2], &lfl[3]);
	if (2 > res)
		error("Linemarkers error scanning line# and file name");
	memset(lm_flags, 0, sizeof(lm_flags));
	for (i = 0; i < res - 2; i++) {
		lm_flags[lfl[i]-1] = TRUE;
	}
	col = 1;
	return TRUE;
}

static bool pp(void)
{
	error("Preprocessor feature is not implemented");
	return FALSE;
}

static bool expect_pp(void)
{
	if (1 == col) {
		if (expect('#')) {
			if (expect(' ')) {
				return linemarkers();
			} else {
				return pp();
			}
		}
	}
	return FALSE;
}

static void skip_wsp(void)
{
	int ch;
	do {
		ch = gch();
	} while (is_whitespace(ch) || '\n' == ch);
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
	buf[sz] = ch;
	/* no ungch() at the end, because it will return the closing "'" */
	return scan_cconst(buf, sz, cnst);
}

extern void scan(void)
{
	do {
		struct tok tok;
		skip_wsp();
		if (expect_pp())
			continue;
		if (expect(EOF)) {
			break;
		}
		if (is_comment_start()) {
			ignore_comment();
			continue; /* above: no token generated */
		} else if (expect_delim(&(tok.val.delim))) {
			tok.type = DELIM_TOK;
		} else if (expect_operator(&(tok.val.op))) {
			tok.type = OP_TOK;
		} else if (expect_cconst(&(tok.val.cnst))) {
			tok.type = CONST_TOK;
		} else if (expect_alphanum(&tok)) {
		} else {
			char err_msg[] = "X: Unknown symbol";
			err_msg[0] = getchar();
			error(err_msg);
		}
		emit_token(&tok);
	}
	while (TRUE);
}

int main(void)
{
	scan();
	return 0;
}
