#ifndef __DELIM_H__
#define __DELIM_H__

enum delim {
	SEMICOLON_DELIM = ';',
	COLON_OP = ':',
	LBRACE_DELIM = '{',
	RBRACE_DELIM = '}',
	LSQBR_DELIM = '[',
	RSQBR_DELIM = ']',
	LPAR_DELIM = '(',
	RPAR_DELIM = ')'
};

#define IS_DELIM(X) (\
		     (X==':') ||\
		     (X==';') ||\
		     (X=='{') ||\
		     (X=='}') ||\
		     (X=='[') ||\
		     (X==']') ||\
		     (X=='(') ||\
		     (X==')')\
		     )

#endif /* __DELIM_H__ */
