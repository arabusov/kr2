#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "bool.h"
#include "tok.h"

#define COL_LIM 511

static enum scan_state { S_COMMENT, S_DQ, S_SQ, S_ESC, S_OTHER, S_EOF };

static enum scan_state state = S_OTHER;

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

int is_proper_idf(char *c)
{
	if (!is_letter(*c))
		return 0;
	c++;
	while (*c != '\0') {
		if (!is_digit(*c) && !is_letter(*c))
			return FALSE;
		c++;
	}
	return TRUE;
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

static void ignore_comment(void)
{
	int ch;
	assert(state == S_COMMENT);
	do {
		ch = gch();
		if (ch == '*' && state == S_COMMENT) {
			ch = gch();
			if (ch == -1) {
				state = S_EOF;
				return;
			}
			if (ch == '/') {
				state = S_OTHER;
				return;
			}
		}
	} while (ch != -1);
	state = S_OTHER;
}

static bool is_comment_start(int *ch)
{
	*ch = gch();
	if (*ch == '/') {
		*ch = gch();
		if (*ch == -1) {
			state = S_EOF;
			return FALSE;
		}
		if (*ch == '*') {
			state = S_COMMENT;
			return TRUE;
		}
	}
	return FALSE;
}

bool expect(const char c)
{
	if (c == gch())
		return TRUE;
	ungetc(c, stdin);
	return FALSE;
}

bool expect_delim(int *ch, enum delim *delim)
{
	*ch = gch();
	if (IS_DELIM(*ch)) {
		*delim = *ch;
		return TRUE;
	}
	ungetc(*ch, stdin);
	return FALSE;
}

#define EXP_NEXT(NEXT, AO)\
	if (expect(NEXT)) { \
		*op = AO; \
		return TRUE; \
	}
extern bool expect_operator(int *ch, enum op *op)
{
	switch (*ch) {
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
	return FALSE;
}

void skip_wsp(int *const ch)
{
	if (!is_whitespace(*ch))
		return;
	do {
		*ch = gch();
	} while (is_whitespace(*ch));
	ungetc(*ch, stdin);
}

#define NTOKMAX 2048
struct tok tok_table[NTOKMAX];
int toki = 0;

extern void scan(void)
{
	int ch;
	do {
		struct tok *tokp = &tok_table[toki];
		if (is_comment_start(&ch)) {
			ignore_comment();
			if (ch == EOF)
				break;
			continue;
		}
		skip_wsp(&ch);
		if (expect_delim(&ch, &(tokp->val.delim))) {
			tokp->type = DELIM_TOK;
		} else if (expect_operator(&ch, (enum op *)&(tokp->val.op))) {
			tokp->type = OP_TOK;
		} else {
			continue;
		}
		printf("%d\n", tokp->type);
		if (toki < NTOKMAX)
			toki++;
	}
	while (ch != EOF);
}

int main(void)
{
	scan();
	return 0;
}
