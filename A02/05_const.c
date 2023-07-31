#include <assert.h>
#include "types.h"
#include "lims.h"
#include "05_const.h"

enum base { NOINT=0, DEC, OCT, HEX };
enum suff { NOSUF, USUF, LSUF, ULSUF};

static void set_itype_nosuf(tulong val, enum base bs, struct cnst *cn)
{
        if (val <= TINT_MAX) {
                cn->type.int_type = INT_CONST;
                cn->val.int_val.int_val = (tint)val;
        } else if (((bs == OCT) || (bs == HEX)) && val <= TUINT_MAX) {
                cn->type.int_type = UINT_CONST;
                cn->val.int_val.uint_val = (tuint)val;
        } else if (val <= TLONG_MAX) {
                cn->type.int_type = LONG_CONST;
                cn->val.int_val.long_val = (tlong)val;
        } else {
                cn->type.int_type = ULONG_CONST;
                cn->val.int_val.ulong_val = val;
        }
}

static void set_itype_suf(tulong val, enum suff sf, struct cnst *cn)
{
        assert((sf == USUF) || (sf == LSUF));
        if ((sf == USUF) && (val <= TUINT_MAX)) {
                cn->type.int_type = UINT_CONST;
                cn->val.int_val.uint_val = (tuint)val;
                return;
        }
        if ((sf == LSUF) && (val <= TLONG_MAX)) {
                cn->type.int_type = LONG_CONST;
                cn->val.int_val.long_val = (tlong)val;
                return;
        }
        cn->type.int_type = ULONG_CONST;
        cn->val.int_val.ulong_val = val;
}

static void set_itype(tulong val, enum base bs, enum suff sf, struct cnst *cn)
{
        cn->type.type = I_CONST;
        switch (sf) {
                case NOSUF:
                        set_itype_nosuf(val, bs, cn); break;
                case USUF: case LSUF:
                        set_itype_suf(val, sf, cn); break;
                case ULSUF:
                        cn->type.int_type = ULONG_CONST;
                        cn->val.int_val.ulong_val = val;
        }
}

#define UC(ch) (ch+('A'-'a'))
#define LC(ch) (ch+('a'-'A'))
#define ISA(x,lc) ((src[sz-x] == lc) || (src[sz-x] == UC(lc)))
static enum suff det_suff(char *src, size_t sz)
{
        if (sz < 2)
                return NOSUF;
        if (sz > 2)
                if ((ISA(2, 'l') && ISA(1,'u')) || (ISA(2, 'u') && ISA(1,'l')))
                        return ULSUF;
        assert(sz >= 2);
        if (ISA(1,'l'))
                return LSUF;
        if (ISA(1,'u'))
                return USUF;
        return NOSUF;
}

static size_t trunk_size(size_t sz, enum suff sf)
{
        switch (sf) {
                case ULSUF: sz -= 2; break;
                case USUF: case LSUF: sz--; break;
                case NOSUF: break;
        }
        return sz;
}

#define TULONG_BIT sizeof(tulong)/sizeof(tchar)*TCHAR_BIT

int ovflow_16(int digit, int bpi)
{
        if (TULONG_BIT-bpi == 3)
                if (digit > 7)
                        return 1;
        if (TULONG_BIT-bpi == 2)
                if (digit > 3)
                        return 1;
        if (TULONG_BIT-bpi == 1)
                if (digit > 1)
                        return 1;
        return 0;
}

int is_digit_8_16(char c, int bpd)
{
        if (bpd == 3)
                if ((c >= '0') && (c <= '7'))
                        return 1;
        if (bpd == 4)
                if (
                                ((c >= '0') && (c <= '9')) ||
                                ((c >= 'a') && (c <= 'f')) ||
                                ((c >= 'A') && (c <= 'F')))
                        return 1;
        return 0;
}

int digit_8_16(char c, int bpd)
{
        if (bpd == 3)
                return c - '0';
        if (bpd == 4) {
                if ((c >= '0') && (c <= '9'))
                        return c - '0';
                else if ((c >= 'a') && (c <= 'f'))
                        return c - 'a' + 10;
                else if ((c >= 'A') && (c <= 'F'))
                        return c - 'A' + 10;
        }
        return 0;
}

enum ddr { FAIL, CONTINUE, LAST };
static enum ddr
dp2d(char c, int bpd, int *bpi, tulong *bs, tulong *res)
{
        int digit;
        tulong delta;
        if (!is_digit_8_16(c, bpd))
                return FAIL;
        digit = digit_8_16(c, bpd);
        if (ovflow_16(digit, *bpi))
                return FAIL;
        if ((TULONG_BIT <= *bpi) && (digit != 0))
                return FAIL;
        if ((TULONG_BIT <= *bpi) && (digit == 0))
                return CONTINUE;
        delta = *bs * digit;
        if (*res <= (TULONG_MAX-delta))
                *res += delta;
        else
                return FAIL;
        *bpi += bpd;
        *bs <<= bpd;
        return CONTINUE;
}

static enum ddr ddd(char c, tulong *bs, tulong *res)
{
        int dig;
        tulong delta;
        if (c < '0' || c > '9')
                return FAIL;
        dig = c - '0';
        if (TULONG_MAX/(*bs) < dig)
                return FAIL;
        delta = *bs * dig;
        if (*res <= (TULONG_MAX - delta))
                *res += delta;
        else
                return FAIL;
        if (TULONG_MAX/(*bs) < 10)
                return LAST;
        *bs *= 10;
        return CONTINUE;
}

static int bpd(enum base cbase)
{
        switch (cbase) {
                case OCT: return 3;
                case HEX: return 4;
                default: return 0;
        }
}

static int decode(char *src, int src_size, struct cnst *cn, enum base cbase)
{
        char c = 0;
        enum suff sf;
        int bpi = 0;
        tulong bs=1, res=0;
        sf = det_suff(src, src_size);
        src_size = trunk_size(src_size, sf) - 1;
        do {
                enum ddr dd;
                c=src[src_size--];
                if (cbase == OCT || cbase == HEX)
                        dd = dp2d(c, bpd(cbase), &bpi, &bs, &res);
                else if (cbase == DEC)
                        dd = ddd(c, &bs, &res);
                else return 0;
                switch (dd) {
                        case FAIL: return 0;
                        case CONTINUE: continue;
                        case LAST: if (src_size > 0)
                                           return 0;
                                   else continue;
                }
        } while (src_size>=0);
        set_itype(res, cbase, sf, cn);
        return 1;
}

static enum base det_base(char *src, size_t src_size)
{
        if (src_size == 0)
                return NOINT;
        if (src_size > 2)
                if ((src[0] == '0') && ((src[1] == 'x') || (src[1] == 'X')))
                        return HEX;
        assert(src_size >= 2);
        if (src[0] == '0')
                return OCT;
        return DEC;
}

int scan_iconst(char *src, size_t src_size, struct cnst *cn)
{
        enum base cbase = det_base(src, src_size);
        switch(cbase) {
                case OCT:
                        return decode(src+1, src_size-1, cn, cbase);
                case HEX:
                        return decode(src+2, src_size-2, cn, cbase);
                case DEC: 
                        return decode(src, src_size, cn, cbase);
                case NOINT:
                        return 0;
        }
        return 0;
}

int scan_cconst(char *src, size_t sz, struct cnst *cn)
{
        if (sz < 2)
                return 0;
        if (src[0] != '\'' && src[sz-1] != '\'')
                return 0;
        src++;
        sz--;
        return 0;
}
