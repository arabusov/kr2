#include <stdio.h>
#include "04_keyw.h"
#include "tests.h"

static struct ktest {
	char *str;
	int res;
	enum keyw class;
} tests[] = {
	{ "auto", TRUE, AUTO },
	{ "break", TRUE, BREAK },
	{ "case", TRUE, CASE },
	{ "const", TRUE, CONST },
	{ "continue", TRUE, CONTINUE },
	{ "default", TRUE, DEFAULT },
	{ "do", TRUE, DO },
	{ "double", TRUE, DOUBLE },
	{ "else", TRUE, ELSE },
	{ "enum", TRUE, ENUM },
	{ "extern", TRUE, EXTERN },
	{ "float", TRUE, FLOAT },
	{ "for", TRUE, FOR },
	{ "goto", TRUE, GOTO },
	{ "if", TRUE, IF },
	{ "int", TRUE, INT },
	{ "long", TRUE, LONG },
	{ "register", TRUE, REGISTER },
	{ "return", TRUE, RETURN },
	{ "short", TRUE, SHORT },
	{ "signed", TRUE, SIGNED },
	{ "sizeof", TRUE, SIZEOF },
	{ "static", TRUE, STATIC },
	{ "struct", TRUE, STRUCT },
	{ "switch", TRUE, SWITCH },
	{ "typedef", TRUE, TYPEDEF },
	{ "union", TRUE, UNION },
	{ "unsigned", TRUE, UNSIGNED },
	{ "void", TRUE, VOID },
	{ "volatile", TRUE, VOLATILE },
	{ "while", TRUE, WHILE },
};

static int single_kwtest(struct ktest *tup)
{
	enum keyw kw;
	int res = scan_keyw(tup->str, &kw);
	if (tup->res == res)
		return TRUE;
	if (res)
		if (tup->class == kw)
			return TRUE;
	return FALSE;
}

extern int test_keyw(void)
{
	unsigned long n_tests = sizeof(tests) / sizeof(struct ktest);
	unsigned i, n_fails = 0;
	for (i = 0; i < n_tests; i++) {
		if (single_kwtest(tests + i) == FALSE) {
			printf("(%u/%lu) failed. User input is %s\n",
			       i + 1, n_tests, (tests + i)->str);
			n_fails++;
		}
	}
	return n_fails == 0;
}
