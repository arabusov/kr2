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
                case USUF: case LSUF: sz --; break;
                case NOSUF: break;
        }
        return sz;
}

#define BPO 3 /* bits per an octal digit */
#define TULONG_BIT sizeof(tulong)/sizeof(tchar)*TCHAR_BIT
static int decode_oct(char *src, size_t src_size, struct cnst *cn)
{
        char c = 0;
        enum suff sf;
        tulong bs=1, res=0, delta;
        sf = det_suff(src, src_size);
        src_size = trunk_size(src_size, sf);
        while (c=src[--src_size], src_size>=1) {
                if ((c < '0') || (c > '7'))
                        return 0;
                delta = bs * (tulong)(c - '0');
                if (res <= (TULONG_MAX-delta))
                        res += delta;
                else
                        return 0;
                bs <<= BPO;
        }
        set_itype(res, OCT, sf, cn);
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
        switch(det_base(src, src_size)) {
                case OCT:
                        if(decode_oct(src, src_size, cn))
                                return 1;
                case DEC: case HEX: case NOINT: default:
                        return 0;
                        break;
        }
        return 0;
}
