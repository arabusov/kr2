#include <stdio.h>
#include <stdarg.h>

#include "parser.h"
#include "debug.h"

extern void print_node(struct node *node)
{
        if (node == NULL) {
                printf("Node is NULL\n");
                return;
        }
        switch (node->type) {
                case PLUS: printf("op+ node  "); break;
                case MUL: printf("op* node  "); break;
                case OPAND: printf("opand node (%d)", node->operand); break;
                default: printf("0x%3x node", node->type); break;
        }
        if (node->left != NULL)
                printf (" left = 0x%8x", (unsigned int)node->left);
        if (node->right!= NULL)
                printf (" right = 0x%8x", (unsigned int)node->right);
        printf("\n");
}

static void print_e(struct node *node, int shift)
{
        int i;
        for (i = 0; i < shift; i++) {
                putchar(' ');
        }
        switch (node->type) {
                case PLUS: putchar('+'); break;
                case MUL: putchar('*'); break;
                case OPAND: printf("%d", node->operand); break;
                default: printf("op(%d)", node->type); break;
        }
        putchar('\n');
        if (node->left != NULL)
                print_e(node->left, shift+4);
        if (node->right != NULL)
                print_e(node->right, shift+4);
}

extern void print_expr(struct node *node)
{
        if (node == NULL) {
                printf("Node is NULL\n");
                return;
        }
        print_e(node, 0); 
}

#ifdef DEBUG
extern void PARSE_DEBUG(const char *fmt, ...)
{
        va_list args;
        va_start(args, fmt);
       
        printf("[PARSE_DEBUG] ");
        while (*fmt != '\0')
        {
                if (fmt[0] == '%' && fmt[1] == 'd') {
                        int dec = va_arg(args, int);
                        printf("%d", dec);
                        fmt += 2;
                } else if (fmt[0] == '%' && fmt[1] == 'x') {
                        int dec = va_arg(args, int);
                        printf("%x", dec);
                        fmt += 2;
                } else {
                        putchar(*fmt);
                        fmt += 1;
                }
        }
        va_end(args);
        putchar('\n');
}
#endif
