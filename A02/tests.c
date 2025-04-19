#include <stdio.h>
#include "tests.h"

#define TEST(f) ((f() == TRUE) ? 1 : 0)
int main(void)
{
	int n_tests = 2;
	int n_succ = 0;
	n_succ += TEST(test_iconst);
	n_succ += TEST(test_chrconst);
	printf("Total test stat: [%d/%d]\n", n_succ, n_tests);
	return 0;
}
