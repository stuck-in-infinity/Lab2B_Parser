#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>

typedef struct ast_node AST;

typedef enum {
    NODE_PROGRAM,
    NODE_STMT_LIST,
    NODE_VAR_DECL,
    NODE_ASSIGNMENT,
    NODE_IF,
    NODE_WHILE,
    NODE_BLOCK,
    NODE_BIN_OP,
    NODE_UNARY_OP,
    NODE_INTEGER,
    NODE_IDENTIFIER
} NodeType;

typedef enum {
    OP_PLUS, OP_MINUS, OP_MUL, OP_DIV,
    OP_EQ, OP_NE, OP_LT, OP_GT, OP_LE, OP_GE
} BinOp;

typedef enum {
    UNARY_PLUS, UNARY_MINUS
} UnaryOp;

struct ast_node {
    NodeType type;
    union {
        struct {
            AST **children;
            int count;
            int capacity;
        } list;

        struct {
            char *name;
            AST *initializer; /* may be NULL */
        } var_decl;

        struct {
            char *name;
            AST *expr;
        } assignment;

        struct {
            AST *condition;
            AST *then_branch;
            AST *else_branch; /* may be NULL */
        } if_stmt;

        struct {
            AST *condition;
            AST *body;
        } while_stmt;

        struct {
            BinOp op;
            AST *left;
            AST *right;
        } bin_op;

        struct {
            UnaryOp op;
            AST *expr;
        } unary_op;

        int int_value;
        char *identifier;
    } data;
};

/* construction */
AST *create_program(AST *stmt_list);
AST *create_stmt_list(void);
void stmt_list_append(AST *list, AST *stmt);

AST *create_var_decl(char *name, AST *init);
AST *create_assignment(char *name, AST *expr);
AST *create_if_stmt(AST *cond, AST *then_s, AST *else_s);
AST *create_while_stmt(AST *cond, AST *body);
AST *create_block(AST *stmt_list);

AST *create_bin_op(BinOp op, AST *l, AST *r);
AST *create_unary_op(UnaryOp op, AST *e);
AST *create_integer(int v);
AST *create_identifier(char *name);

/* utilities */
void print_ast(AST *node, int indent);
void free_ast(AST *node);

#endif
