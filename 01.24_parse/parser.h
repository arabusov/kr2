#ifndef __PARSER_H__
#define __PARSER_H__
enum node_type { PLUS, MUL, OPAND };

struct node {
	enum node_type type;
	int operand;
	struct node *left;
	struct node *right;
};

#endif /* __PARSER_H__ */
