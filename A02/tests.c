#include <stdio.h>
#include "tests.h"

int main(void)
{
        int n_tests = 1, n_succ = 0;
        n_succ += test_iconst() ? 1 : 0;
        printf("Total test stat: [%d/%d]\n", n_succ, n_tests);
        return 0;
}
