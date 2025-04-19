#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "tests.h"
#include "05_const.h"

#define TRUE 1
#define FALSE 0

#define BUFLEN 24
struct tup {
	unsigned long in_val;
	int is_int_const;
	int base;
	enum iconst_type ct;
	char in_str[BUFLEN];
	tulong val;
};

static void conf_tup(struct tup *s, int base)
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
		printf(" fail: %lu (in) != %lu (out) ", i, o);
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
	}
	return FALSE;
}

struct tup tp[] = {
	{ 0, FALSE, 10, INT_CONST, "400094967296" },
	{ 017777, TRUE, 8, INT_CONST },
	{ 0x7bcd, TRUE, 16, INT_CONST },
	{ 1264, TRUE, 10, INT_CONST },
	{ 1284, TRUE, 10, INT_CONST },
	{ 32767, TRUE, 10, INT_CONST },
	{ 0x7fff, TRUE, 10, INT_CONST, "0x7fff" },
	{ 077777, TRUE, 10, INT_CONST, "077777" },
	{ 32767, TRUE, 10, LONG_CONST, "32767l" },
	{ 32767, TRUE, 10, UINT_CONST, "32767u" },
	{ 32767, TRUE, 10, UINT_CONST, "32767U" },
	{ 32767, TRUE, 10, ULONG_CONST, "32767ul" },
	{ 65535, TRUE, 10, LONG_CONST },
	{ 0xffff, TRUE, 10, UINT_CONST, "0xffff" },
	{ 65535l, TRUE, 10, LONG_CONST, "65535l" },
	{ 65535u, TRUE, 10, UINT_CONST, "65535u" },
	{ 65536, TRUE, 10, LONG_CONST },
	{ 2147483647, TRUE, 10, LONG_CONST },
	{ 2147483647u, TRUE, 10, ULONG_CONST, "2147483647u" },
	{ 2147483648, TRUE, 10, ULONG_CONST },
	{ 4294967295, TRUE, 10, ULONG_CONST },
	{ 0, FALSE, 10, INT_CONST, "4294967296" },
	{ 0, FALSE, 10, INT_CONST, "4000094967296" },
	{ 0, FALSE, 0, INT_CONST, "0x100000000" },
	{ 0xffffffff, TRUE, 16, ULONG_CONST },
	{ 0x12fb, TRUE, 16, INT_CONST },
	{ 0x7fff, TRUE, 16, INT_CONST },
	{ 0x8000, TRUE, 16, UINT_CONST, "0x8000" },
	{ 0, FALSE, 0, INT_CONST, "08000" },
	{ 0, TRUE, 8, INT_CONST, "0000" },
	{ 0, TRUE, 10, INT_CONST, "0" },
	{ 0x0000, TRUE, 16, INT_CONST, "0x0000" },
	{ 01, TRUE, 8, INT_CONST, "01" },
	{ 1, TRUE, 10, INT_CONST, "1" },
	{ 0x0001, TRUE, 16, INT_CONST, "0x0001" },
	{ 0xabcd, TRUE, 16, UINT_CONST, "0xabcd" },
	{ 0x12fbu, TRUE, 16, UINT_CONST, "0x12fbu" },
	{ 0xabcdef, TRUE, 16, LONG_CONST, "0xabcdef" },
	{ 0x12fbl, TRUE, 16, LONG_CONST, "0x12fbl" },
	{ 0x12fblu, TRUE, 16, ULONG_CONST, "0x12fblu" },
	{ 0x12fbul, TRUE, 16, ULONG_CONST, "0x12fbul" },
	{ 0x12fbUL, TRUE, 16, ULONG_CONST, "0x12fbUL" },
	{ 0x12FbUL, TRUE, 16, ULONG_CONST, "0x12FbUL" },
	{ 0x12FbUl, TRUE, 16, ULONG_CONST, "0x12FbUl" },
	{ 0x12F00blU, TRUE, 16, ULONG_CONST, "0x12F00blU" },
	{ 0x12F00b, TRUE, 16, LONG_CONST, "0x12F00b" },
	{ 0, FALSE, 0, INT_CONST, "076543212345" },
	{ 0, FALSE, 0, INT_CONST, "066543212345" },
	{ 0, FALSE, 0, INT_CONST, "056543212345" },
	{ 0, FALSE, 0, INT_CONST, "040000000000" },
	{ 037777777777, TRUE, 8, ULONG_CONST, "037777777777" },
	{ 017777777777, TRUE, 8, LONG_CONST, "017777777777" },
	{ 036543212345, TRUE, 8, ULONG_CONST, "036543212345" },
	{ 012345, TRUE, 8, INT_CONST, "012345" },
	{ 077777, TRUE, 8, INT_CONST, "077777" },
	{ 0177777, TRUE, 8, UINT_CONST, "0177777" },
	{ 0177777u, TRUE, 8, UINT_CONST, "0177777u" },
	{ 0177777l, TRUE, 8, LONG_CONST, "0177777l" },
	{ 0177777ul, TRUE, 8, ULONG_CONST, "0177777ul" },
	{ 0177777lu, TRUE, 8, ULONG_CONST, "0177777lu" },
	{ 01777777, TRUE, 8, LONG_CONST, "01777777" },
	{ 01777777l, TRUE, 8, LONG_CONST, "01777777l" },
	{ 01777777ul, TRUE, 8, ULONG_CONST, "01777777ul" },
	{ 01777777lu, TRUE, 8, ULONG_CONST, "01777777lu" },
	{ 01777777lu, TRUE, 8, ULONG_CONST, "0001777777lu" },
	{ 0, FALSE, 0, INT_CONST, "0777777777777" }
};

extern int test_iconst(void)
{
	struct cnst cn;
	unsigned long n_t = sizeof(tp) / sizeof(struct tup);
	unsigned long n_succ = 0;
	int i;
	for (i = 0; i < n_t; i++) {
		int a1, a2, a3 = FALSE;
		int res;
		char *s;
		unsigned long tp_len;
		if (tp[i].is_int_const && tp[i].in_str[0] == '\0'
		    && tp[i].base != 0) {
			conf_tup(&tp[i], tp[i].base);
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
