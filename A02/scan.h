#ifndef _SCAN_H_
#define _SCAN_H_
#include "tok.h"

extern void scan(struct tok *tok);
extern void error(const char *s);

#endif /* _SCAN_H_ */
