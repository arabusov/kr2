#include <assert.h>
#include "table.h"

char *table[TABLE_LEN];
int tablei = 0;

void table_push(char *const name)
{
	assert(tablei < TABLE_LEN);
	table[tablei] = name;
	tablei++;
}
