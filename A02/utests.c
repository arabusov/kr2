#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "const.h"
#include "keyw.h"
#include "bool.h"
#include "types.h"

#define BUFLEN 24
struct tup {
	unsigned long in_val;
	int is_int_const;
	int base;
	enum iconst_type ct;
	char in_str[BUFLEN];
	tulong val;
};

static void conf_tup(struct tup *s)
{
	assert(sizeof(unsigned long) >= sizeof(tulong));
	switch (s->base) {
	case 10:
		sprintf(s->in_str, "%lu", (unsigned long)s->in_val);
		break;
	case 16:
		sprintf(s->in_str, "0x%lx", (unsigned long)s->in_val);
		break;
	case 8:
		sprintf(s->in_str, "0%lo", (unsigned long)s->in_val);
		break;
	default:
		fprintf(stderr, "Internal test error\n");
		exit(EXIT_FAILURE);
	}
}

static int asrt_io(tulong i, tulong o)
{
	int res = (i == o);
	if (!res)
		printf(" fail: %" TARGlu " (in) != %" TARGlu " (out) ", i, o);
	return res;
}

static int asrt_v(enum iconst_type t, tulong i1, union int_const *i2)
{
	switch (t) {
	case INT_CONST:
		return asrt_io(i1, i2->int_val);
	case UINT_CONST:
		return asrt_io(i1, i2->uint_val);
	case LONG_CONST:
		return asrt_io(i1, i2->long_val);
	case ULONG_CONST:
		return asrt_io(i1, i2->ulong_val);
	default:
		return FALSE;
	}
	return FALSE;
}

struct tup tp[] = {
	{ 0, FALSE, 10, INT_CONST, "400094967296", 0 },
	{ 017777, TRUE, 8, INT_CONST, "", 0 },
	{ 0x7bcd, TRUE, 16, INT_CONST, "", 0 },
	{ 1264, TRUE, 10, INT_CONST, "", 0 },
	{ 1284, TRUE, 10, INT_CONST, "", 0 },
	{ 32767, TRUE, 10, INT_CONST, "", 0 },
	{ 0x7fff, TRUE, 10, INT_CONST, "0x7fff", 0 },
	{ 077777, TRUE, 10, INT_CONST, "077777", 0 },
	{ 32767, TRUE, 10, LONG_CONST, "32767l", 0 },
	{ 32767, TRUE, 10, UINT_CONST, "32767u", 0 },
	{ 32767, TRUE, 10, UINT_CONST, "32767U", 0 },
	{ 32767, TRUE, 10, ULONG_CONST, "32767ul", 0 },
	{ 65535, TRUE, 10, LONG_CONST, "", 0 },
	{ 0xffff, TRUE, 10, UINT_CONST, "0xffff", 0 },
	{ 65535l, TRUE, 10, LONG_CONST, "65535l", 0 },
	{ 65535u, TRUE, 10, UINT_CONST, "65535u", 0 },
	{ 65536, TRUE, 10, LONG_CONST, "", 0 },
	{ 2147483647, TRUE, 10, LONG_CONST, "", 0 },
	{ 2147483647u, TRUE, 10, ULONG_CONST, "2147483647u", 0 },
	{ 2147483648u, TRUE, 10, ULONG_CONST, "", 0 },
	{ 4294967295u, TRUE, 10, ULONG_CONST, "", 0 },
	{ 0, FALSE, 10, INT_CONST, "4294967296", 0 },
	{ 0, FALSE, 10, INT_CONST, "4000094967296", 0 },
	{ 0, FALSE, 0, INT_CONST, "0x100000000", 0 },
	{ 0xffffffff, TRUE, 16, ULONG_CONST, "", 0 },
	{ 0x12fb, TRUE, 16, INT_CONST, "", 0 },
	{ 0x7fff, TRUE, 16, INT_CONST, "", 0 },
	{ 0x8000, TRUE, 16, UINT_CONST, "0x8000", 0 },
	{ 0, FALSE, 0, INT_CONST, "08000", 0 },
	{ 0, TRUE, 8, INT_CONST, "0000", 0 },
	{ 0, TRUE, 10, INT_CONST, "0", 0 },
	{ 0x0000, TRUE, 16, INT_CONST, "0x0000", 0 },
	{ 01, TRUE, 8, INT_CONST, "01", 0 },
	{ 1, TRUE, 10, INT_CONST, "1", 0 },
	{ 0x0001, TRUE, 16, INT_CONST, "0x0001", 0 },
	{ 0xabcd, TRUE, 16, UINT_CONST, "0xabcd", 0 },
	{ 0x12fbu, TRUE, 16, UINT_CONST, "0x12fbu", 0 },
	{ 0xabcdef, TRUE, 16, LONG_CONST, "0xabcdef", 0 },
	{ 0x12fbl, TRUE, 16, LONG_CONST, "0x12fbl", 0 },
	{ 0x12fblu, TRUE, 16, ULONG_CONST, "0x12fblu", 0 },
	{ 0x12fbul, TRUE, 16, ULONG_CONST, "0x12fbul", 0 },
	{ 0x12fbUL, TRUE, 16, ULONG_CONST, "0x12fbUL", 0 },
	{ 0x12FbUL, TRUE, 16, ULONG_CONST, "0x12FbUL", 0 },
	{ 0x12FbUl, TRUE, 16, ULONG_CONST, "0x12FbUl", 0 },
	{ 0x12F00blU, TRUE, 16, ULONG_CONST, "0x12F00blU", 0 },
	{ 0x12F00b, TRUE, 16, LONG_CONST, "0x12F00b", 0 },
	{ 0, FALSE, 0, INT_CONST, "076543212345", 0 },
	{ 0, FALSE, 0, INT_CONST, "066543212345", 0 },
	{ 0, FALSE, 0, INT_CONST, "056543212345", 0 },
	{ 0, FALSE, 0, INT_CONST, "040000000000", 0 },
	{ 037777777777, TRUE, 8, ULONG_CONST, "037777777777", 0 },
	{ 017777777777, TRUE, 8, LONG_CONST, "017777777777", 0 },
	{ 036543212345, TRUE, 8, ULONG_CONST, "036543212345", 0 },
	{ 012345, TRUE, 8, INT_CONST, "012345", 0 },
	{ 077777, TRUE, 8, INT_CONST, "077777", 0 },
	{ 0177777, TRUE, 8, UINT_CONST, "0177777", 0 },
	{ 0177777u, TRUE, 8, UINT_CONST, "0177777u", 0 },
	{ 0177777l, TRUE, 8, LONG_CONST, "0177777l", 0 },
	{ 0177777ul, TRUE, 8, ULONG_CONST, "0177777ul", 0 },
	{ 0177777lu, TRUE, 8, ULONG_CONST, "0177777lu", 0 },
	{ 01777777, TRUE, 8, LONG_CONST, "01777777", 0 },
	{ 01777777l, TRUE, 8, LONG_CONST, "01777777l", 0 },
	{ 01777777ul, TRUE, 8, ULONG_CONST, "01777777ul", 0 },
	{ 01777777lu, TRUE, 8, ULONG_CONST, "01777777lu", 0 },
	{ 01777777lu, TRUE, 8, ULONG_CONST, "0001777777lu", 0 },
	{ 0, FALSE, 0, INT_CONST, "0777777777777", 0 }
};

extern int test_iconst(void)
{
	struct cnst cn;
	unsigned long n_t = sizeof(tp) / sizeof(struct tup);
	unsigned long n_succ = 0;
	unsigned i;
	for (i = 0; i < n_t; i++) {
		int a1, a2, a3 = FALSE;
		int res;
		char *s;
		unsigned long tp_len;
		if (tp[i].is_int_const && tp[i].in_str[0] == '\0'
		    && tp[i].base != 0) {
			conf_tup(&tp[i]);
		}
		tp_len = strlen(tp[i].in_str);
		s = tp[i].in_str;
		res = scan_iconst(s, tp_len, &cn);
		a1 = asrt_io(tp[i].is_int_const, res);
		if (!a1) {
			printf("[%d/%lu] (1: %s)\n", i + 1, n_t, s);
			continue;
		}
		if (res) {
			a2 = asrt_io(tp[i].ct, cn.type.int_type);
			if (!a2) {
				printf("[%d/%lu] (2: %s)\n", i + 1, n_t, s);
				continue;
			}
			a3 = asrt_v(tp[i].ct, tp[i].in_val, &cn.val.int_val);
			if (!a3) {
				printf("[%d/%lu] (3: %s)\n", i + 1, n_t, s);
				continue;
			}
		}
		n_succ++;
	}
	if (n_succ == n_t) {
		return TRUE;
	}
	printf("Integer constant: failed (%lu/%lu)\n", n_t - n_succ, n_t);
	return n_succ == n_t ? TRUE : FALSE;
}

struct test_tup {
	char *uinp;
	int is_chrconst;
	struct const_type type;
	int val;
};

static int single_test(struct test_tup *tup)
{
	size_t sz = strlen(tup->uinp);
	struct cnst cn;
	int res = scan_cconst(tup->uinp, sz, &cn);
	if (tup->is_chrconst == res)
		return TRUE;
	if (res)		/* also test the result value */
		if ((tchar) tup->val == (tchar) cn.val.char_val)
			return TRUE;
	return FALSE;
}

static struct test_tup char_tests[] = {
	{ "\'a\'", 1, { CH_CONST, NOT_INT_CONST}, 'a' },
	{ "\'?\'", 1, { CH_CONST, NOT_INT_CONST}, '?' },
	{ "\'ab\'", 0, { NOT_CONST, NOT_INT_CONST}, '\0' },
	{ "\'\\n\'", 1, { CH_CONST, NOT_INT_CONST}, '\n' },
	{ "\'\\v\'", 1, { CH_CONST, NOT_INT_CONST}, '\v' },
	{ "\'\\a\'", 1, { CH_CONST, NOT_INT_CONST}, '\a' },
	{ "\'\\t\'", 1, { CH_CONST, NOT_INT_CONST}, '\t' },
	{ "\'\\n\'", 1, { CH_CONST, NOT_INT_CONST}, '\n' },
	{ "\'\\?\'", 1, { CH_CONST, NOT_INT_CONST}, '\?' },
	{ "\'\\0\'", 1, { CH_CONST, NOT_INT_CONST}, '\0' },
	{ "\'\\1\'", 1, { CH_CONST, NOT_INT_CONST}, '\1' },
	{ "\'\\12\'", 1, { CH_CONST, NOT_INT_CONST}, '\12' },
};

extern int test_chrconst(void)
{
	unsigned long n_tests = sizeof(char_tests) / sizeof(struct test_tup);
	unsigned i, n_fails = 0;
	for (i = 0; i < n_tests; i++) {
		if (single_test(char_tests + i) == FALSE) {
			printf("(%u/%lu) failed. User input is %s\n",
			       i + 1, n_tests, (char_tests + i)->uinp);
			n_fails++;
		}
	}
	return n_fails == 0;
}

static struct ktest {
	char *str;
	int res;
	enum keyw class;
} keyw_tests[] = {
	{ "alignas", FALSE, NOT_KEYW },	/* C23 */
	{ "alignof", FALSE, NOT_KEYW },	/* C23 */
	{ "auto", TRUE, AUTO },
	{ "bool", FALSE, NOT_KEYW },	/* C23 */
	{ "break", TRUE, BREAK },
	{ "case", TRUE, CASE },
	{ "const", TRUE, CONST },
	{ "constexpr", FALSE, NOT_KEYW },	/* C23 */
	{ "continue", TRUE, CONTINUE },
	{ "default", TRUE, DEFAULT },
	{ "do", TRUE, DO },
	{ "double", TRUE, DOUBLE },
	{ "else", TRUE, ELSE },
	{ "enum", TRUE, ENUM },
	{ "extern", TRUE, EXTERN },
	{ "false", FALSE, NOT_KEYW },	/* C23 */
	{ "float", TRUE, FLOAT },
	{ "for", TRUE, FOR },
	{ "goto", TRUE, GOTO },
	{ "if", TRUE, IF },
	{ "int", TRUE, INT },
	{ "inline", FALSE, NOT_KEYW },	/* C99 */
	{ "long", TRUE, LONG },
	{ "nullptr", FALSE, NOT_KEYW },	/* C23 */
	{ "register", TRUE, REGISTER },
	{ "return", TRUE, RETURN },
	{ "restrict", FALSE, NOT_KEYW },	/* C99 */
	{ "short", TRUE, SHORT },
	{ "signed", TRUE, SIGNED },
	{ "sizeof", TRUE, SIZEOF },
	{ "static", TRUE, STATIC },
	{ "static_assert", FALSE, NOT_KEYW },	/* C23 */
	{ "struct", TRUE, STRUCT },
	{ "switch", TRUE, SWITCH },
	{ "thread_local", FALSE, NOT_KEYW },	/* C23 */
	{ "true", FALSE, NOT_KEYW },	/* C23 */
	{ "typedef", TRUE, TYPEDEF },
	{ "typeof", FALSE, NOT_KEYW },	/* C23 */
	{ "typeof_unqual", FALSE, NOT_KEYW },	/* C23 */
	{ "union", TRUE, UNION },
	{ "unsigned", TRUE, UNSIGNED },
	{ "void", TRUE, VOID },
	{ "volatile", TRUE, VOLATILE },
	{ "while", TRUE, WHILE },
	{ "_Bool", FALSE, NOT_KEYW },	/* C99 */
	{ "_Complex", FALSE, NOT_KEYW },	/* C99 */
	{ "_Imaginary", FALSE, NOT_KEYW },	/* C99 */
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
	unsigned long n_tests = sizeof(keyw_tests) / sizeof(struct ktest);
	unsigned i, n_fails = 0;
	for (i = 0; i < n_tests; i++) {
		if (single_kwtest(keyw_tests + i) == FALSE) {
			printf("(%u/%lu) failed. User input is %s\n",
			       i + 1, n_tests, (keyw_tests + i)->str);
			n_fails++;
		}
	}
	return n_fails == 0;
}

#define TEST(f) (n_tests++, (f() == TRUE) ? 1 : 0)

int main(void)
{
	int n_tests = 0;
	int n_succ = 0;
	n_succ += TEST(test_iconst);
	n_succ += TEST(test_chrconst);
	n_succ += TEST(test_keyw);
	printf("Total test stat: [%d/%d]\n", n_succ, n_tests);
	return 0;
}
