#include "ident.h"
#include "table.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void save_ident(const char *src)
{
	char *storage;
	size_t sz;
	sz = strlen(src);
	assert(sz <= IDENT_LEN);
	storage = (char *)malloc(sz + 1);
	assert(storage != NULL);
	strcpy(storage, src);
	table_push(storage);
}
