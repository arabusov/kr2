#ifndef __IDENT_H__
#define __IDENT_H__

#include <stddef.h>

#define IDENT_LEN 31

typedef char *ident;

char *save_ident(const char *, size_t);

#endif /* __IDENT_H__ */
