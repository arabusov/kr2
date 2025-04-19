#include <stdlib.h>

#include "error.h"
#include "parser.h"
#include "bool.h"

#include "debug.h"

struct node *create_node(enum node_type type, int operand, struct node *l,
			 struct node *r)
{
	struct node *node = (struct node *)malloc(sizeof(struct node));
	node->type = type;
	node->operand = operand;
	node->left = l;
	node->right = r;
	return node;
}

void delete_node(struct node *node)
{
	free(node);
}

enum lex_t { LEX_NULL = 0300, LEX_CONST };

struct lexema {
	int type;
	int val;
};

struct lexema stream[] = {
	{ '(', 0 },
	{ '(', 0 },
	{ LEX_CONST, 1 },
	{ '+', 0 },
	{ LEX_CONST, 5 },
	{ ')', 0 },
	{ '+', 0 },
	{ LEX_CONST, 2 },
	{ '*', 0 },
	{ '(', 0 },
	{ LEX_CONST, 2 },
	{ '+', 0 },
	{ LEX_CONST, 3 },
	{ ')', 0 },
	{ ')', 0 },
	{ '*', 0 },
	{ LEX_CONST, 4 }
};

int prio(int op)
{
	switch (op) {
	case '+':
		return 0;
	case '*':
		return 1;
	default:
		return -1;
	}
	return -1;
}

int ssize = sizeof(stream) / sizeof(struct lexema);

struct node *parse_expr(int lp, int rp, struct node *ln, struct node *rn);

struct node *build_par(int lp, int rp, int q, int p)
{
	struct node *prt;
	PARSE_DEBUG("build_par(%d, %d, %d, %d)", lp, rp, q, p);
	/* corner cases */
	if (q == lp + 1) {
		error("Wrong placement of (");
		return NULL;
	}
	if (rp == p + 1) {
		error("Wrong placement of )");
		return NULL;
	}
	prt = parse_expr(q + 1, p - 1, NULL, NULL);
	if (lp == q && rp == p)
		return prt;
	if (lp > q || rp < p) {
		error("Internal error");
		return NULL;
	}
	if (lp == q)
		return parse_expr(p + 1, p + 1, prt,
				  parse_expr(p + 2, rp, NULL, NULL));
	if (rp == p)
		return parse_expr(q - 1, q - 1,
				  parse_expr(lp, q - 2, NULL, NULL), prt);
	if (prio(stream[p + 1].type) > prio(stream[q - 1].type))
		return parse_expr(q - 1, q - 1,
				  parse_expr(lp, q - 2, NULL, NULL),
				  parse_expr(p + 1, p + 1,
					     prt,
					     parse_expr(p + 2, rp, NULL,
							NULL)));
	return parse_expr(p + 1, p + 1, parse_expr(p + 2, rp, NULL, NULL),
			  parse_expr(q - 1, q - 1,
				     parse_expr(lp, q - 2, NULL, NULL), prt));
}

int find_bra(int lp, int rp, int p)
{
	int q, cnt = 0;
	for (q = p - 1; q >= lp; q--) {
		PARSE_DEBUG("find_bra(%d, %d, %d, %d, %d)", lp, rp, p, q, cnt);
		if (stream[q].type == ')') {
			cnt++;
		} else if (stream[q].type == '(') {
			if (cnt == 0)
				return q;
			cnt--;
		}
	}
	error("Unmatched ) at %d", p);
	return -1;
}

struct node *par(int lp, int rp)
{
	int p;
	for (p = rp; p >= lp; p--) {
		if (stream[p].type == ')') {
			int q = find_bra(lp, rp, p);
			if (q < 0)
				return NULL;
			return build_par(lp, rp, q, p);
		}
	}
	return NULL;
}

struct node *binary(int lp, int rp, int op)
{
	int p;
	for (p = rp; p >= lp; p--) {
		if (stream[p].type == op)
			return parse_expr(p, p,
					  parse_expr(lp, p - 1, NULL, NULL),
					  parse_expr(p + 1, rp, NULL, NULL));
	}
	return NULL;
}

struct node *bop_single(struct node *ln, struct node *rn, enum node_type op)
{
	if (ln == NULL || rn == NULL) {
		error("BINARY OP must have 2 arguments");
		return NULL;
	}
	return create_node(op, 0, ln, rn);;
}

struct node *parse_single(int lp, struct node *ln, struct node *rn)
{
	switch (stream[lp].type) {
	case LEX_CONST:
		if (ln != NULL || rn != NULL) {
			error("OPAND cannot have arguments");
			return NULL;
		}
		return create_node(OPAND, stream[lp].val, ln, rn);
	case '+':
		return bop_single(ln, rn, PLUS);
	case '*':
		return bop_single(ln, rn, MUL);
	case '(':
		error("Unmatched (");
		return NULL;
	case ')':
		error("Unmatched )");
		return NULL;
	default:
		return NULL;
	}
}

struct node *parse_expr(int lp, int rp, struct node *ln, struct node *rn)
{
	struct node *res = NULL;
	PARSE_DEBUG("parse_expr(%d, %d, 0x%x, 0x%x)", lp, rp,
		    (unsigned)ln, (unsigned)rn);
	if (lp > rp)
		return NULL;
	if (lp == rp)
		return parse_single(lp, ln, rn);
	res = par(lp, rp);
	if (res == NULL)
		res = binary(lp, rp, '+');
	if (res == NULL)
		res = binary(lp, rp, '*');
	return res;
}

int eval_expr(struct node *expr)
{
	int val;
	switch (expr->type) {
	case OPAND:
		if (expr->left != NULL || expr->right != NULL) {
			error("OPAND has arguments");
			exit(-1);
		}
		return expr->operand;
	case PLUS:
		val = eval_expr(expr->right) + eval_expr(expr->left);
		break;
	case MUL:
		val = eval_expr(expr->right) * eval_expr(expr->left);
		break;
	default:
		error("Unknown TYPE for EVAL");
		exit(-1);
	}
	PARSE_DEBUG("eval_expr() = %d", val);
	return val;
}

#include <stdio.h>
void print_stream(void)
{
	int i;
	printf("0...............1...............\n");
	printf("0123456789abcdef0123456789abcdef\n");
	for (i = 0; i < ssize; i++) {
		if (stream[i].type == LEX_CONST)
			printf("%d", stream[i].val);
		else
			putchar(stream[i].type);
	}
	putchar('\n');
}

int main(void)
{
	struct node *expr;
	print_stream();
	expr = parse_expr(0, ssize - 1, NULL, NULL);
	print_expr(expr);
	if (!error_state)
		printf("%d\n", eval_expr(expr));
	else
		printf("Error while parsing: %d\n", error_state);
	return 0;
}
