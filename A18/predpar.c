/*
 * ASU, Example 4.14
 * Grammar:
 * S -> cAd
 * A -> ab | a
 */

#include <stdio.h>
#include <stdlib.h>

void error(const char *s)
{
	fprintf(stderr, "Error: %s\n", s);
	exit(1);
}

void scan(int *tok)
{
	*tok = getchar();
	switch (*tok) {
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case EOF:
		case '\n':
			break;
		default:
			fprintf(stderr, "%d: ", *tok);
			error("unknown token");
	}
}

#define BLEN 1024
int buffer[BLEN];
int sp = -1;
int bp = -1;

int lookahead = EOF;

void next()
{
	if (sp >= (BLEN-1))
		error("Input buffer overflown");
	if (sp < bp) {
		lookahead = buffer[sp++];
		return;
	}
	scan(&buffer[++sp]);
	bp = sp;
	lookahead = buffer[sp];
}

void pop()
{
	if (sp < 0)
		error("Input buffer is empty");
	sp--;
	lookahead = buffer[sp];
}

/*
 * S -> cAd
 * A -> ab | a
 */

int A()
{
	if ('a' == lookahead) {
		next();
		if ('b' == lookahead) {
			next();
			return 1;
		}
		return 1;
	}
	return 0;
}

int S()
{
	if ('c' == lookahead) {
		next();
		if (A()) {
			if ('d' == lookahead) {
				next();
				return 1;
			}
			pop();
		}
		pop();
	}
	return 0;
}

void match(int tok)
{
	if (tok == lookahead) {
		next();
		return;
	}
	error("could not match token");
}

void parse()
{
	next(); /* initialize lookahead and buffer */

	while (EOF != lookahead) {
		if (S()) {
			match('\n');
			printf("Matched\n");
			continue;
		} else {
			error("Parse error");
		}
	}
}

int main()
{
	int res, i;
	parse();
	return 0;
}
