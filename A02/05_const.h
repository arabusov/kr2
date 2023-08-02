#ifndef __CONST_H__
#define __CONST_H__

#include "types.h"
#include <stddef.h>

union int_const {
        tint    int_val;
        tuint   uint_val;
        tlong   long_val;
        tulong  ulong_val;
};

union const_val {
        tchar                   char_val;
        union int_const         int_val;
        tchar*                  str_val;
        tint                    enum_val;
};

enum iconst_type { INT_CONST, UINT_CONST, LONG_CONST, ULONG_CONST };

struct const_type {
        enum { I_CONST, CH_CONST, STR_CONST, ENUM_CONST } type;
        enum iconst_type        int_type;
};

struct cnst {
        struct const_type       type;
        union const_val         val;
};

extern int scan_iconst(char *, size_t, struct cnst *);
extern int scan_cconst(char *, size_t, struct cnst *);

#endif /* __CONST_H__ */
