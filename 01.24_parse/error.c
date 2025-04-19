#include <stdio.h>
#include <stdarg.h>

#include "bool.h"
#include "error.h"

int error_state = FALSE;

extern void error(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	while (*fmt != '\0') {
		if (fmt[0] == '%' && fmt[1] == 'd') {
			int dec = va_arg(args, int);
			fprintf(stdout, "%d", dec);
			fmt += 2;
		} else {
			fputc(*fmt, stdout);
			fmt += 1;
		}
	}
	va_end(args);
	fputc('\n', stdout);
	error_state = TRUE;
}
