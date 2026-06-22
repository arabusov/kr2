#include "ident.h"
#include "table.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

char *save_ident(const char *src, size_t sz)
{
	char *storage;
	assert(sz <= IDENT_LEN);
	assert(sz > 0);
	storage = (char *)malloc(sz + 1);
	assert(storage != NULL);
	memcpy(storage, src, sz);
	storage[sz] = '\0';
	table_push(storage);
	return storage;
}
