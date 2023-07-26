#include "types.h"
#include "lims.h"
#include "05_const.h"

#define TULONG_BIT sizeof(tulong)/sizeof(tchar)*TCHAR_BIT

enum base { NOINT=0, DEC, OCT, HEX };

void set_itype(tulong val, enum base bs, struct cnst *cn)
{
        cn->type.type = I_CONST;
        if (val < TINT_MAX) {
                cn->type.int_type = INT_CONST;
                cn->val.int_val.int_val = (tint)val;
        } else if (((bs == OCT) || (bs == HEX)) && val < TUINT_MAX) {
                cn->type.int_type = UINT_CONST;
                cn->val.int_val.uint_val = (tuint)val;
        } else if (val < TLONG_MAX) {
                cn->type.int_type = LONG_CONST;
                cn->val.int_val.long_val = (tlong)val;
        } else {
                cn->type.int_type = ULONG_CONST;
                cn->val.int_val.ulong_val = val;
        }
}

int decode_oct(char *src, size_t src_size, struct cnst *cn)
{
        char c = 0;
        tulong bs=1, res=0, delta;
        if (src_size >= TULONG_BIT)
                return 0;
        while (c=src[--src_size], src_size>=1) {
                if ((c < '0') || (c > '7'))
                        return 0;
                delta = bs * (tulong)(c - '0');
                if (res < (TULONG_MAX-delta))
                        res += delta;
                else
                        return 0;
                bs <<= 3;
        }
        set_itype(res, OCT, cn);
        return 1;
}

static enum base det_base(char *src, size_t src_size)
{
        if (src_size == 0)
                return NOINT;
        if (src_size > 2)
                if (src[0] == '0')
                        if ((src[1] == 'x') || (src[1] == 'X'))
                                return HEX;
                        else
                                return OCT;
                else
                        return DEC;
        else
                if (src[0] == '0')
                        return OCT;
                else
                        return DEC;
}

int scan_iconst(char *src, size_t src_size, struct cnst *cn)
{
        switch(det_base(src, src_size)) {
                case OCT:
                        if(decode_oct(src, src_size, cn)) {
                                return 1;
                        }
                case DEC: case HEX: case NOINT: default:
                        return 0;
                        break;
        }
        return 0;
}
