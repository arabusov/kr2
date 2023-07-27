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

#define BUFLEN 48
int test_const(void)
{
        struct cnst cn;
        char todec[][BUFLEN] = {
                "017777",
                "017777u",
                "017777l",
                "017777ul",
                "017777lu",
                "0177777",
                "0177777u",
                "0177777l",
                "0177777lu",
                "0177777ul"
        };
        int i;
        for (i = 0; i < sizeof(todec)/BUFLEN; i++) {
                size_t todec_len=strlen(todec[i]);
                int res=scan_iconst(todec[i], todec_len, &cn);
                if (res) {
                        printf("%s passed\n", todec[i]);
                        printf("Type %d: ", cn.type.type);
                        printf("iType %d: ", cn.type.int_type);
                        print_val(&cn);
                } else {
                        printf("%s failed\n", todec[i]);
                }
        }
        return 0;
}

int main(void)
{
        test_const();
        return 0;
}
