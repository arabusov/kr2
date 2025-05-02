#ifndef __TABLE_H__
#define __TABLE_H__

#include "ident.h"
#define TABLE_LEN 2048

extern char *table[TABLE_LEN];
extern int tablei;

extern void table_push(char *const);

#endif /* __TABLE_H__ */
