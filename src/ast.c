#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

ASTNode *make_node(NodeType type) {
    ASTNode *n = calloc(1, sizeof(ASTNode));
    if (!n) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    n->type = type;
    return n;
}

ASTNode *make_number(int v) {
    ASTNode *n = make_node(NODE_NUMBER);
    n->value = v;
    return n;
}

ASTNode *make_ident(char *s) {
    ASTNode *n = make_node(NODE_IDENT);
    if (s)
        n->name = strdup(s);
    return n;
}

ASTNode *make_binop(char *op, ASTNode *l, ASTNode *r) {
    ASTNode *n = make_node(NODE_BINOP);
    if (op)
        n->name = strdup(op);
    n->left = l;
    n->right = r;
    return n;
}

static void indent(int n) {
    for (int i = 0; i < n; i++)
        printf("  ");
}

void print_ast(ASTNode *n, int depth) {
    if (!n)
        return;

    indent(depth);

    switch (n->type) {
        case NODE_PROGRAM:
            printf("PROGRAM\n");
            if (n->body)
                print_ast(n->body, depth + 1);
            break;

        case NODE_BLOCK:
            printf("BLOCK\n");
            if (n->body)
                print_ast(n->body, depth + 1);
            break;

        case NODE_VAR_DECL:
            printf("VAR_DECL(%s)\n", n->name ? n->name : "?");
            if (n->right) {
                indent(depth + 1);
                printf("INIT:\n");
                print_ast(n->right, depth + 2);
            }
            break;

        case NODE_ASSIGN:
            printf("ASSIGN(%s)\n", n->name ? n->name : "?");
            if (n->right) {
                indent(depth + 1);
                printf("VALUE:\n");
                print_ast(n->right, depth + 2);
            }
            break;

        case NODE_IF:
            printf("IF\n");
            if (n->cond) {
                indent(depth + 1);
                printf("CONDITION:\n");
                print_ast(n->cond, depth + 2);
            }
            if (n->body) {
                indent(depth + 1);
                printf("THEN:\n");
                print_ast(n->body, depth + 2);
            }
            if (n->else_body) {
                indent(depth + 1);
                printf("ELSE:\n");
                print_ast(n->else_body, depth + 2);
            }
            break;

        case NODE_WHILE:
            printf("WHILE\n");
            if (n->cond) {
                indent(depth + 1);
                printf("CONDITION:\n");
                print_ast(n->cond, depth + 2);
            }
            if (n->body) {
                indent(depth + 1);
                printf("BODY:\n");
                print_ast(n->body, depth + 2);
            }
            break;

        case NODE_BINOP:
            printf("BINOP(%s)\n", n->name ? n->name : "?");
            if (n->left) {
                indent(depth + 1);
                printf("LEFT:\n");
                print_ast(n->left, depth + 2);
            }
            if (n->right) {
                indent(depth + 1);
                printf("RIGHT:\n");
                print_ast(n->right, depth + 2);
            }
            break;

        case NODE_NUMBER:
            printf("NUMBER(%d)\n", n->value);
            break;

        case NODE_IDENT:
            printf("IDENT(%s)\n", n->name ? n->name : "?");
            break;

        default:
            printf("UNKNOWN_NODE\n");
            break;
    }

    if (n->next)
        print_ast(n->next, depth);
}
