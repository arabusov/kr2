#include <stdio.h>
#include <string.h>
#include "05_const.h"

void print_val(struct cnst *cn)
{
        switch(cn->type.type) {
        case I_CONST:
                switch(cn->type.int_type) {
                case INT_CONST:
                        printf("val %d\n", cn->val.int_val.int_val); break;
                case UINT_CONST:
                        printf("val %d\n", cn->val.int_val.uint_val); break;
                case LONG_CONST:
                        printf("val %d\n", cn->val.int_val.long_val); break;
                case ULONG_CONST:
                        printf("val %d\n", cn->val.int_val.ulong_val); break;
                } break;
        case CH_CONST:
                printf("val %c\n", cn->val.char_val); break;
        case STR_CONST:
                printf("val %s\n", cn->val.str_val); break;
        case ENUM_CONST:
                printf("val %d\n", cn->val.enum_val); break;
        }
}

int test_const(void)
{
        struct cnst cn;
        char *todec="0177777ul";
        size_t todec_len=strlen(todec);
        int res=scan_iconst(todec, todec_len, &cn);
        if (res) {
                printf("%s passed\n", todec);
                printf("Type %d: ", cn.type.type);
                printf("iType %d: ", cn.type.int_type);
                print_val(&cn);
        } else {
                printf("%s failed\n", todec);
        }
        return 0;
}

int main(void)
{
        test_const();
        return 0;
}
