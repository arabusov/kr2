/*
 * A18.1, lvalue
 * Grammar:
 * EL	: e
 *	| e , EL
 *	;
 * ELo	: EL
 *	| ""
 *	;
 * P	: i
 *	| c
 *	| s
 *	| ( e )
 *	| P ( ELo )
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
 * The grammar contains left recursion.
 *
 * 1) We eliminate immediate left recursion in L:
 *
 * lvalue
 * L	: i RL | P [ e ] RL | P - i RL | * e RL | ( L ) RL
 * RL	: . i RL | ""
 *
 * 2) Then, we replace the only production of P -> L . i RP wiht the current L
 * production:
 *
 * P	: i
 *	| c
 *	| s
 *	| ( e )
 *	| i RL . i
 *	| ( L ) RL . i
 *	| * e RL . i
 *	| P ( ELo )
 *	| P [ e ]
 *	| P [ e ] RL . i
 *	| P - i RL . i
 *	| P - i
 *	;
 *
 * 3) We eliminate immediate left recursion in P:
 * P	: i RP
 *	| c RP
 *	| s RP
 *	| ( e ) RP
 *	| i RL . i RP
 *	| ( L ) RL . i RP
 *	| * e RL . i RP
 *	;
 * RP	: ( ELo ) RP
 *	| [ e ] RP
 *	| [ e ] RL . i RP
 *	| - i RL . i RP
 *	| - i RP
 *	| ""
 *	;
 
 *
 * The procedure does not eliminate left recursion involving derivations of two
 * or more steps.
 */

#include <stdio.h>
#include <stdlib.h>

void error(const char *s)
{
	fprintf(stderr, "Error: %s\n", s);
	exit(1);
}

void print_node(const char *s, int offset)
{
	while (offset--)
		putchar(' ');
	puts(s);
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
		case 's':
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

void print_buffer()
{
        int i;
        printf("bp = %d, sp = %d\n", bp, sp);
        for (i = 0; i < sp + 1; i ++) {
                if (sp != i)
                        printf("    ");
                else
                        printf("--->");
                printf("%d(%c)\n", buffer[i], (char)buffer[i]);
        }
}

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

void rw_stack(int old_sp)
{
        sp = old_sp;
        lookahead = buffer[sp];
}

/* EL	: e
 *	| e , EL */
int ELo()
{
	if ('e' == lookahead) {
		next();
		if (',' == lookahead) {
			next();
			if (ELo()) {
				print_node("ELo	-> e , ELo", sp);
				return 1;
			}
			pop();
			/* fall through, because reasons */
			/* buffer points to ',' */
		}
		print_node("ELo -> e", sp);
		/* buffer may or may not point to ',', but 'e' is matched  */
		return 1;
	}
	/* match epsilon */
	return 1;
}

/*
 * primary
 */
int RP();
int dotiRP()
{
	if ('.' == lookahead) {
		next();
		if ('i' == lookahead) {
			next();
			if (RP()) {
				print_node(" e", sp);
				return 1;
			}
			pop();
		}
		pop();
	}
	return 0;
}

/* RP	: ( ELo ) RP
 *	| [ e ] RP
 *	| [ e ] RL . i RP
 *	| - i RL . i RP
 *	| - i RP
 *	| ""
 *	;
 */
 
int RL();	/* forward declaration */
int L();	/* forward declaration */

int RP()
{
	int old_sp = -1;
	if ('(' == lookahead) {
		next();
		/* | ( ELo ) RP */
		old_sp = sp;
		if (ELo()) {
			if (RP())
				return 1;
		}
		/* despite succ EL, need to rw_stack back if RP fails */
		rw_stack(old_sp);
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
				if (RL())
					if (dotiRP())
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
			int old_sp = sp;
			next();
			if (RL())
				if (dotiRP())
					return 1;
			rw_stack(old_sp);
			if (RP())
				return 1;
		}
		pop();
	}
	/* | "" */
	return 1;
}

/*
 * P	: i RP
 *	| c RP
 *	| s RP
 *	| ( e ) RP
 *	| i RL . i RP
 *	| ( L ) RL . i RP
 *	| * e RL . i RP
 *	;
 */

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
		default: break;
	}
	return 0;
}

int RLdotiRP()
{
	int old_sp = sp;
	if (RL())
		if (dotiRP())
			return 1;
	rw_stack(old_sp);
	return 0;
}

/*
 * P	: P1 RP
 *	| i RL . i RP
 *	| ( L ) RL . i RP
 *	| * e RL . i RP
 *	;
 */

int P()
{
	int old_sp;
	old_sp = sp;
	printf("P -> P1 RP : sp=%d\n", sp);
	if (P1()) {
		if (RP())
			return 1;
	}
	rw_stack(old_sp);
	printf("P -> RL . i RP : sp=%d\n", sp);
	if (RLdotiRP())
		return 1;
	rw_stack(old_sp);
	printf("P -> ( L ) RP : sp=%d\n", sp);
	if ('(' == lookahead)
	{
		next();
		if (L())
			if (')' == lookahead)
				if (RLdotiRP())
					return 1;
	}
	rw_stack(old_sp);
	printf("P -> * e RL . i RP : sp=%d\n", sp);
	if ('*' == lookahead) {
		old_sp = sp;
		next();
		if ('e' == lookahead) {
			next();
			if (RLdotiRP())
				return 1;
		}
		rw_stack(old_sp);
	}

	return 0;
}

/*
 * lvalue
 * L	: i RL | P [ e ] RL | P - i RL | * e RL | ( L ) RL
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
	old_sp = sp;
	/* L -> i RL */
	printf("L -> i RL : sp=%d\n", sp);
	if ('i' == lookahead) {
		next();
		if (RL()) {
			print_node("L -> i RL", sp);
			return 1;
		}
		pop();
        }
        /* L -> P [ e ] */
	rw_stack(old_sp);
	if (P()) {
		printf("L -> P [ e ] : sp=%d\n", sp);
		if ('[' == lookahead) {
			next();
			if ('e' == lookahead) {
				next();
				if (']' == lookahead) {
					next();
					if (RL()) {
						print_node("L -> P [ e ]", sp);
						return 1;
					}
				}
				pop();
			}
			pop();
		}
		/* L -> P - i RL */
		printf("L -> P - i RL : sp=%d\n", sp);
		if ('-' == lookahead) {
			next();
			if ('i' == lookahead) {
				next();
				if (RL())
					return 1;
			}
			pop();
		}
	}
	rw_stack(old_sp); /* if P() but rest failed */
	/* * e RL */
	printf("L -> * e RL : sp=%d\n", sp);
	if ('*' == lookahead) {
		next();
		if ('e' == lookahead) {
			next();
			if (RL())
				return 1;
		}
		pop();
	}
	/* ( L ) RL */
	printf("L -> ( L ) RL : sp=%d\n", sp);
	if ('(' == lookahead) {
		next();
		old_sp = sp;
		if (L()) {
			if (')' == lookahead) {
				next();
				if (RL()) {
					print_node("L -> ( L ) RL", sp);
					return 1;
				}
				pop();
			}
		}
		rw_stack(old_sp);
	}
	return 0;
}

void match(int tok)
{
	if (tok == lookahead) {
		next();
		return;
	}
        print_buffer();
	error("could not match token");
}

void parse()
{
	next(); /* initialize lookahead and buffer */

	while (EOF != lookahead) {
		if (L()) {
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
