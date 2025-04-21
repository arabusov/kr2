#ifndef __OP_H__
#define __OP_H__

#include "bool.h"

enum op {
	PLUS_OP = '+',
	MINUS_OP = '-',
	AST_OP = '*',
	DIV_OP = '/',
	REM_OP = '%',
	NEG_OP = '~',
	AND_OP = '&',
	OR_OP = '|',
	XOR_OP = '^',
	DOT_OP = '.',
	COMMA_OP = ',',
	QMARK_OP = '?',
	NOT_OP = '!',
	ASSIGN_OP = '=',
	LESS_OP = '<',
	GR_OP = '>',

	RS_OP,			/* right shift */
	LS_OP,			/* left shift */

	LEQ_OP,
	GEQ_OP,

	ARROW_OP,

	ASSIGNPL_OP,
	ASSIGNMN_OP,
	ASSIGNML_OP,
	ASSIGNDV_OP,
	ASSIGNRM_OP,
	ASSIGNAN_OP,
	ASSIGNOR_OP,
	ASSIGNXR_OP,
	ASSIGNRS_OP,
	ASSIGNLS_OP,

	INC_OP,
	DEC_OP,

	LAND_OP,		/* logical and */
	LOR_OP,

	EQ_OP,
	NEQ_OP,

};

bool is_operator(const char *uin, size_t, enum op *);

#endif /* __OP_H__ */
