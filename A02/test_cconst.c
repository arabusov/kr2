#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "05_const.h"
#include "tests.h"

#define BUFLEN 512

struct test_tup {
        char                    uinp[BUFLEN];
        int                     is_chrconst;
        struct const_type       type;
        int                     val;
};

int single_test(struct test_tup *tup)
{
        size_t sz = strlen(tup->uinp);
        struct cnst cn;
        int res = scan_cconst(tup->uinp, sz, &cn);
        if (tup->is_chrconst == res)
                return TRUE;
        return FALSE;
}

struct test_tup tests[] = {
        {"\'a\'",        1,     {CH_CONST},     'a'},
        {"\'?\'",        1,     {CH_CONST},     '?'},
        {"\'ab\'",       0},
        {"\'\\n\'",      1,     {CH_CONST},     '\n'},
        {"\'\\v\'",      1,     {CH_CONST},     '\v'},
        {"\'\\a\'",      1,     {CH_CONST},     '\a'},
        {"\'\\t\'",      1,     {CH_CONST},     '\t'},
        {"\'\\n\'",      1,     {CH_CONST},     '\n'},
        {"\'\\?\'",      1,     {CH_CONST},     '\?'},
        {"\'\\0\'",      1,     {CH_CONST},     '\0'},
        {"\'\\1\'",      1,     {CH_CONST},     '\1'},
        {"\'\\12\'",     1,     {CH_CONST},     '\12'},
};

extern int test_chrconst(void)
{
        unsigned long n_tests = sizeof(tests)/sizeof(struct test_tup);
        unsigned i, n_fails=0;
        for (i = 0; i < n_tests; i++) {
                if (single_test(tests+i) == FALSE) {
                        printf("(%u/%lu) failed. User input is %s\n",
                                        i + 1, n_tests, (tests+i)->uinp);
                        n_fails++;
                }
        }
        return n_fails == 0;
}
