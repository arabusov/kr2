#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tests.h"
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

static void conf_tup(struct tup *s, int base)
{
        char *dummy;
        s->val = strtol(s->num, &dummy, base);
}

static int asrt_io(unsigned long i, unsigned long o)
{
        int res = (i == o);
        if (!res)
                printf(" fail: %lu != %lu ", i, o);
        return res;
}

static int asrt_v(enum iconst_type t, long i1, union int_const *i2)
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

extern int test_iconst(void)
{
        struct cnst cn;
        struct tup tp[] = {
                {"400000094967296",  FALSE, 10},
                {"017777",      TRUE, 8,        INT_CONST},
                {"0x7bcd",      TRUE, 16,       INT_CONST},
                {"1264",        TRUE, 10,       INT_CONST},
                {"1284",        TRUE, 10,       INT_CONST},
                {"32767",       TRUE, 10,       INT_CONST}, 
                {"32767l",      TRUE, 10,       LONG_CONST}, 
                {"32767u",      TRUE, 10,       UINT_CONST}, 
                {"32767U",      TRUE, 10,       UINT_CONST}, 
                {"32767ul",     TRUE, 10,       ULONG_CONST}, 
                {"65535",       TRUE, 10,       LONG_CONST},
                {"65535l",      TRUE, 10,       LONG_CONST},
                {"65535u",      TRUE, 10,       UINT_CONST},
                {"65536",       TRUE, 10,       LONG_CONST},
                {"2147483647",  TRUE, 10,       LONG_CONST},
                {"2147483648",  TRUE, 10,       ULONG_CONST},
                {"4294967295",  TRUE, 10,       ULONG_CONST},
                {"4294967296",  FALSE, 10},
                {"4000094967296",  FALSE, 10},
                {"0x100000000", FALSE, 10},
                {"0xfffffffffffffffffffffffffffffffffu", FALSE, 16},
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
        size_t n_t = sizeof(tp)/sizeof(struct tup), n_succ=0;
        int i;
        for (i = 0; i < n_t; i++) {
                int a1, a2, a3 = FALSE;
                char *s = tp[i].num;
                size_t tp_len=strlen(s);
                int res=scan_iconst(s, tp_len, &cn);
                conf_tup(&tp[i], tp[i].base);
                a1 = asrt_io(tp[i].is_int_const, res);
                if (!a1) {
                        printf("[%d/%lu] (1: %s)\n", i+1, n_t, s);
                        continue;
                }
                if (res) {
                        a2 = asrt_io(tp[i].ct, cn.type.int_type);
                        if (!a2) {
                                printf("[%d/%lu] (2: %s)\n", i+1, n_t, s);
                                continue;
                        }
                        a3 = asrt_v(tp[i].ct, tp[i].val, &cn.val.int_val);
                        if (!a3) {
                                printf("[%d/%lu] (3: %s)\n", i+1, n_t, s);
                                continue;
                        }
                }
                n_succ++;
        }
        if (n_succ == n_t) {
                return TRUE;
        }
        printf("Integer constant: failed (%lu/%lu)\n", n_t-n_succ, n_t);
        return n_succ == n_t ? TRUE : FALSE;
}
