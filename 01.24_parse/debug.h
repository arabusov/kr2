#ifndef __DEBUG_H__
#define __DEBUG_H__
#include "parser.h"

#ifdef DEBUG
extern void PARSE_DEBUG(const char *fmt, ...);
#else
#define PARSE_DEBUG(...)
#endif /* DEBUG */
extern void print_node(struct node *node);
extern void print_expr(struct node *node);

#endif /* __DEBUG_H__ */
