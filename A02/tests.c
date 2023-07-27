#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "05_const.h"

#define TRUE 1
#define FALSE 0

void print_val(struct cnst *cn)
{
        switch(cn->type.type) {
        case I_CONST:
                switch(cn->type.int_type) {
                case INT_CONST:
                        printf("val %d\n", cn->val.int_val.int_val); break;
                case UINT_CONST:
                        printf("val %d\n", cn->val.int_val.uint_val); break;
                case LONG_CONST:
                        printf("val %d\n", cn->val.int_val.long_val); break;
                case ULONG_CONST:
                        printf("val %d\n", cn->val.int_val.ulong_val); break;
                } break;
        case CH_CONST:
                printf("val %c\n", cn->val.char_val); break;
        case STR_CONST:
                printf("val %s\n", cn->val.str_val); break;
        case ENUM_CONST:
                printf("val %d\n", cn->val.enum_val); break;
        }
}

#define BUFLEN 48
struct tup
{
        char num[BUFLEN];
        int     is_int_const;
        enum iconst_type ct;
        union int_const val;
};

void conf_tup(struct tup *s)
{
        char *dummy;
        tulong val = strtol(s->num, &dummy, 8);
        switch (s->ct) {
                case INT_CONST:
                        s->val.int_val = (tint)val; break;
                case UINT_CONST:
                        s->val.uint_val = (tuint)val; break;
                case LONG_CONST:
                        s->val.long_val = (tlong)val; break;
                case ULONG_CONST:
                        s->val.ulong_val = val; break;
        }
}

int assert_io(unsigned long i, unsigned long o)
{
        int res = (i == o);
        if (!res)
                printf("fail: %lu != %lu ", i, o);
        return res;
}

int assert_val(enum iconst_type t, union int_const *i1, union int_const *i2)
{
        switch (t) {
                case INT_CONST:
                        return assert_io(i1->int_val, i2->int_val);
                case UINT_CONST:
                        return assert_io(i1->uint_val, i2->uint_val);
                case LONG_CONST:
                        return assert_io(i1->long_val, i2->long_val);
                case ULONG_CONST:
                        return assert_io(i1->ulong_val, i2->ulong_val);
        }
        return FALSE;
}

int test_const(void)
{
        struct cnst cn;
        struct tup todec[] = {
                {"017777",      TRUE, INT_CONST},
                {"017777u",     TRUE, UINT_CONST},
                {"017777l",     TRUE, LONG_CONST},
                {"017777ul",    TRUE, ULONG_CONST},
                {"017777lu",    TRUE, ULONG_CONST},
                {"0177777",     TRUE, UINT_CONST},
                {"0177777l",    TRUE, LONG_CONST},
                {"0177777ul",   TRUE, ULONG_CONST},
                {"0177777lu",   TRUE, ULONG_CONST},
        };
        int i;
        for (i = 0; i < sizeof(todec)/sizeof(struct tup); i++) {
                int a1, a2, a3 = FALSE;
                size_t todec_len=strlen(todec[i].num);
                int res=scan_iconst(todec[i].num, todec_len, &cn);
                conf_tup(&todec[i]);
                printf("case %s: ", todec[i].num);
                a1 = assert_io(todec[i].is_int_const, res);
                if (a1)
                        a2 = assert_io(todec[i].ct, cn.type.int_type);
                else
                        printf ("(a1 test)");
                if (a1 && a2)
                        a3 = assert_val(todec[i].ct, &todec[i].val, &cn.val.int_val);
                else
                        printf("(a2 test)");
                if (!a3)
                        printf("(a3 test)");
                else
                        printf("\tpassed");
                printf("\n");
        }
        return 0;
}

int main(void)
{
        test_const();
        return 0;
}
