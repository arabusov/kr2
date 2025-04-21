#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "bool.h"
#include "01_tok.h"

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
	int old;
	*ch = gch();
	old = *ch;
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

static void scan(void)
{
	int ch;
	do {
		if (is_comment_start(&ch))
			ignore_comment();
	} while (ch != EOF);
}

int main(void)
{
	scan();
	return 0;
}
