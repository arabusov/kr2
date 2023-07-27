#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "05_const.h"

#define TRUE 1
#define FALSE 0
#define BUFLEN 512

struct tup
{
        char                    num[BUFLEN];
        int                     is_int_const;
        int                     base;
        enum iconst_type        ct;
        long                    val;
};

void conf_tup(struct tup *s, int base)
{
        char *dummy;
        s->val = strtol(s->num, &dummy, base);
}

int assert_io(unsigned long i, unsigned long o)
{
        int res = (i == o);
        if (!res)
                printf(" fail: %lu != %lu ", i, o);
        return res;
}

int assert_val(enum iconst_type t, long i1, union int_const *i2)
{
        switch (t) {
                case INT_CONST:
                        return assert_io(i1, i2->int_val);
                case UINT_CONST:
                        return assert_io(i1, i2->uint_val);
                case LONG_CONST:
                        return assert_io(i1, i2->long_val);
                case ULONG_CONST:
                        return assert_io(i1, i2->ulong_val);
        }
        return FALSE;
}

int test_const(void)
{
        struct cnst cn;
        struct tup todec[] = {
                {"017777",      TRUE, 8,        INT_CONST},
                {"0x7bcd",      TRUE, 16,       INT_CONST},
                {"0x100000000", FALSE},
                {"0xfffffffffffffffffffffffffffffffffu", FALSE},
                {"0xffffffff",  TRUE, 16,       ULONG_CONST},
                {"0x12fb",      TRUE, 16,       INT_CONST},
                {"0x7fff",      TRUE, 16,       INT_CONST},
                {"0x8000",      TRUE, 16,       UINT_CONST},
                {"0xabcd",      TRUE, 16,       UINT_CONST},
                {"0x12fbu",     TRUE, 16,       UINT_CONST},
                {"0xabcdef",    TRUE, 16,       LONG_CONST},
                {"0x12fbl",     TRUE, 16,       LONG_CONST},
                {"0x12fblu",    TRUE, 16,       ULONG_CONST},
                {"0x12fbul",    TRUE, 16,       ULONG_CONST},
                {"0x12fbUL",    TRUE, 16,       ULONG_CONST},
                {"0x12FbUL",    TRUE, 16,       ULONG_CONST},
                {"0x12FbUl",    TRUE, 16,       ULONG_CONST},
                {"0x12F00blU",  TRUE, 16,       ULONG_CONST},
                {"0x12F00b",    TRUE, 16,       LONG_CONST},
                {"076543212345",FALSE},
                {"066543212345",FALSE},
                {"056543212345",FALSE},
                {"040000000000",FALSE},
                {"037777777777",TRUE, 8,        ULONG_CONST},
                {"036543212345",TRUE, 8,        ULONG_CONST},
                {"012345",      TRUE, 8,        INT_CONST},
                {"017777u",     TRUE, 8,        UINT_CONST},
                {"017777l",     TRUE, 8,        LONG_CONST},
                {"017777ul",    TRUE, 8,        ULONG_CONST},
                {"017777lu",    TRUE, 8,        ULONG_CONST},
                {"0177777",     TRUE, 8,        UINT_CONST},
                {"0177777l",    TRUE, 8,        LONG_CONST},
                {"0177777ul",   TRUE, 8,        ULONG_CONST},
                {"0177777lu",   TRUE, 8,        ULONG_CONST},
                {"000000000000000000000000000000000000000000000000000000000000\
000000000000000000000000000000000000000000000000000000000000000000000000000017\
7777lu",                        TRUE, 8,        ULONG_CONST},
                {"017777777777",TRUE, 8,        LONG_CONST},
                {"037777777777",TRUE, 8,        ULONG_CONST},
                {"0777777777777",FALSE}
        };
        size_t n_tests = sizeof(todec)/sizeof(struct tup), n_succ=0;
        int i;
        for (i = 0; i < n_tests; i++) {
                int a1, a2, a3 = FALSE;
                size_t todec_len=strlen(todec[i].num);
                int res=scan_iconst(todec[i].num, todec_len, &cn);
                conf_tup(&todec[i], todec[i].base);
                printf("%d/%lu", i+1, n_tests);
                a1 = assert_io(todec[i].is_int_const, res);
                if (!a1) {
                        printf ("(a1 test for %s)\n", todec[i].num);
                        continue;
                }
                if (res) {
                        a2 = assert_io(todec[i].ct, cn.type.int_type);
                        if (!a2) {
                                printf("(a2 test for %s)\n", todec[i].num);
                                continue;
                        }
                        a3 = assert_val(todec[i].ct, todec[i].val, &cn.val.int_val);
                        if (!a3) {
                                printf("(a3 test for %s)\n", todec[i].num);
                                continue;
                        }
                }
                n_succ++;
                printf("\tpassed\n");
        }
        printf("Passed: %lu/%lu\n", n_succ, n_tests);
        return 0;
}

int main(void)
{
        test_const();
        return 0;
}
