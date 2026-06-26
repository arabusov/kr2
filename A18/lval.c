/*
 * A18.1, lvalue
 * Grammar:
 * EL	: e
 *	| e , EL
 *	;
 * P	: i
 *	| c
 *	| s
 *	| ( e )
 *	| P ( )
 *	| P ( EL )
 *	| P [ e ]
 *	| L . i
 *	| P - i
 *	;
 * L	: i
 *	| P [ e ]
 *	| L . i
 *	| P - i
 *	| * e
 *	| ( L )
 *
 *
 * The grammar contains left recursion. We eliminate it as follows:
 * P1	: i | c | s | ( e )
 * P	: P1 RP | L . i
 * RP	: ( ) RP | ( EL ) RP | [ e ] RP | - i RP | ""
 *
 * lvalue
 * L	: i RL | P [ e ] | P - i | * e | ( L )
 * RL	: . i RL | ""
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
		case '(':
		case ')':
		case '[':
		case ']':
		case '-':
		case '*':
		case '.':
		case 'i':
		case 'c':
		case 'e':
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
	if (sp <= 0)
		error("Input buffer is empty");
	sp--;
	lookahead = buffer[sp];
}

/* EL -> e | e , EL */
int EL()
{
	if ('e' == lookahead) {
		next();
		if (',' == lookahead) {
			if (EL())
				return 1;
			/* if EL fails, the stack muts be restored autom */
		}
		pop();
	}
	return 0;
}

/*
 * primary
 * P1	: i | c | s | ( e )
 * P	: P1 RP | L . i
 * RP	: ( ) RP | ( EL ) RP | [ e ] RP | - i RP | ""
 */

 /* P1	: i | c | s | ( e ) */
int P1()
{
	switch (lookahead) {
		case 'i':
		case 'c':
		case 's':
			next();
			return 1;
		case '(':
			next();
			if ('e' == lookahead) {
				next();
				if (')' == lookahead) {
					next();
					return 1;
				}
				pop();
			}
			pop();
			break;
		default: return 0;
	}
	return 0;
}

 /* RP	: ( ) RP | ( EL ) RP | [ e ] RP | - i RP | "" */
int RP()
{
	int old_sp = -1;
	/* : ( ) RP */
	if ('(' == lookahead) {
		next();
		if (')' == lookahead) {
			next();
			if (RP())
				return 1;
			pop();
		}
		/* | ( EL ) RP */
		old_sp = sp;
		if (EL()) {
			if (RP())
				return 1;
		}
		sp = old_sp; /* despite succ EL, need to rewind back if RP fails*/
	}
	/* | [ e ] RP */
	if ('[' == lookahead) {
		next();
		if ('e' == lookahead) {
			next();
			if (']' == lookahead) {
				next();
				if (RP())
					return 1;
				pop();
			}
			pop();
		}
		pop();
	}
	/* | - i RP  */
	if ('-' == lookahead) {
		next();
		if ('i' == lookahead) {
			next();
			if (RP())
				return 1;
			pop();
		}
		pop();
	}
	/* | "" */
	return 1;
}

/* P	: P1 RP | L . i */
int L(); /* forward declaration */
int P()
{
	int old_sp;
	old_sp = sp;
	if (P1()) {
		if (RP())
			return 1;
	}
	sp = old_sp;
	if (L()) {
		if ('.' == lookahead) {
			next();
			if ('i' == lookahead) {
				next();
				return 1;
			}
			pop();
		}
	}
	sp = old_sp;
	return 0;
}

/*
 * lvalue
 * L	: i RL | P [ e ] | P - i | * e | ( L )
 * RL	: . i RL | ""
 */

 /* RL	: . i RL | "" */
int RL()
{
	if (lookahead == '.') {
		next();
		if (lookahead == 'i') {
			next();
			if (RL()) {
				return 1;
			}
			pop();
		}
		pop();
	}
	return 1;	/* eat up empty string */
}

 /* L	: i RL | P [ e ] | P - i | * e | ( L ) */
int L()
{
	int old_sp;
	if ('i' == lookahead) {
		next();
		if (RL())
			return 1;
		pop();
	}
	old_sp = sp;
	if (P()) {
		/* L -> P [ e ] */
		if ('[' == lookahead) {
			next();
			if ('e' == lookahead) {
				next();
				if (']' == lookahead) {
					next();
					return 1;
				}
				pop();
			}
			pop();
		}
		/* L -> P - i */
		if ('-' == lookahead) {
			next();
			if ('i' == lookahead) {
				next();
				return 1;
			}
			pop();
		}
	}
	sp = old_sp; /* if P() but rest failed */
	/* * e */
	if ('*' == lookahead) {
		next();
		if ('e' == lookahead) {
			next();
			return 1;
		}
		pop();
	}
	/* ( L ) */
	if ('(' == lookahead) {
		next();
		old_sp = sp;
		if (L()) {
			if (')' == lookahead) {
				next();
				return 1;
			}
		}
		sp = old_sp; /* unnecessary if last production */
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
	parse();
	return 0;
}
