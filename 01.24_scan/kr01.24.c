#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>

#define COL_LIM 511
#define TRUE 1
#define FALSE (! TRUE)
#define VBS FALSE

static enum state_t { S_COMMENT, S_DQ, S_SQ, S_ESC, S_OTHER, S_EOF };

static enum state_t state = S_OTHER;

static int lineno = 1, col = 1;

static void error(char *s)
{
	fprintf(stderr, "Error at %d[L]:%d[C] -- %s\n", lineno, col, s);
	exit(EXIT_FAILURE);
}

#define pc(c) { \
        int loc_ch = putchar(c); \
        if (loc_ch == EOF) { \
                error("stdout write failure"); \
        } \
}

#define SSIZE 2048
static char stack[SSIZE];
static ptrdiff_t stackp;

static void push(char x)
{
	assert(stackp >= 0);
	assert(stackp < (SSIZE - 1));
	stack[stackp++] = x;
}

static char pop()
{
	char val;
	assert(stackp >= 1);
	assert(stackp < SSIZE);
	val = stack[--stackp];
	return val;
}

static int unmatched(int bra, int ket)
{
	int a = bra == '(' && ket == ')';
	int b = bra == '[' && ket == ']';
	int c = bra == '{' && ket == '}';
	return !(a || b || c);
}

static void prnth(int ch)
{
	switch (ch) {
	case '(':
	case '{':
	case '[':
		push(ch);
		break;
	case ')':
	case '}':
	case ']':
		if (stackp == 0)
			error("unmatched parenths");
		{
			int bra = pop();
			if (unmatched(bra, ch))
				error("unmatched parenthesis");
		}

	}
}

static void test_at_eof(void)
{
	if (stackp != 0)
		error("unmatched parenthesis at EOF");
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
		error("Colon limit exceeded");
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

static int is_comment_start(int *ch)
{
	int old;
	*ch = gch();
	old = *ch;
	if (*ch == '/') {
		*ch = gch();
		if (*ch == -1) {
			state = S_EOF;
			if (VBS)
				pc(old);
			return FALSE;
		}
		if (*ch == '*') {
			state = S_COMMENT;
			return TRUE;
		}
		if (VBS)
			pc(old);
	}
	if (VBS)
		pc(*ch);
	return FALSE;
}

enum quote_mode { SQM, DQM };
/*
 * Also test for multiline comments of course
 */
static int esc(enum quote_mode qmode)
{
	int cntr = 0;
	int ch;
	do {
		ch = gch();
		if (ch >= '0' && ch <= '7') {
			if (cntr < 3) {
				cntr++;
				continue;
			} else {
				error("escape sequence is too long");
			}
		} else {
			if (ch != '\'') {
				if (cntr > 0)
					error
					    ("unknown digit for an escape sequence");
			} else
				return gch();
			break;
		}
	} while (TRUE);
	switch (ch) {
	case '\?':
	case '\"':
	case '\'':
	case '\\':
	case 'a':
	case 'b':
	case 'f':
	case 'n':
	case 'r':
	case 'v':
	case 't':
		if (qmode == SQM) {
			ch = gch();
			if (ch != '\'')
				error("unmatched single quote in esc");
		}
		return ch;
	default:
		if (qmode == SQM)
			error("unknown escape character");
		else
			return ch;
	}
	return ch;
}

static void dq(void)
{
	int ch;
	do {
		ch = gch();
		if (ch == '\\') {
			ch = esc(DQM);
			if (ch == '\n')
				break;
		}
	} while (ch != '"');
}

static void sq(void)
{
	int ch = gch();
	if (ch == '\\') {
		esc(SQM);
		return;
	}
	if (gch() != '\'')
		error("unmatched single quote");
}

/* run scanner 'with some crazy test inside? */
static void scan(void)
{
	int ch;
	do {
		if (is_comment_start(&ch))
			ignore_comment();
		switch (ch) {
		case '\'':
			sq();
			break;
		case '"':
			dq();
			break;
		case '(':
		case '{':
		case '[':
		case ')':
		case '}':
		case ']':
			prnth(ch);
			break;
		}
	} while (ch != EOF);
}

int main(void)
{
	scan();
	return 0;
}
