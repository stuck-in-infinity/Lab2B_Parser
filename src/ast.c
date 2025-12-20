#include "ast.h"
#include <string.h>

static AST *alloc_node(NodeType t) {
    AST *n = (AST *)malloc(sizeof(AST));
    if (!n) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }
    n->type = t;
    return n;
}

AST *create_stmt_list(void) {
    AST *n = alloc_node(NODE_STMT_LIST);
    n->data.list.count = 0;
    n->data.list.capacity = 4;
    n->data.list.children = (AST **)malloc(sizeof(AST *) * n->data.list.capacity);
    if (!n->data.list.children) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }
    return n;
}

void stmt_list_append(AST *list, AST *stmt) {
    if (!list || list->type != NODE_STMT_LIST || !stmt) return;
    if (list->data.list.count == list->data.list.capacity) {
        list->data.list.capacity *= 2;
        list->data.list.children = (AST **)realloc(
            list->data.list.children,
            sizeof(AST *) * list->data.list.capacity
        );
        if (!list->data.list.children) {
            fprintf(stderr, "Out of memory\n");
            exit(1);
        }
    }
    list->data.list.children[list->data.list.count++] = stmt;
}

AST *create_program(AST *stmt_list) {
    AST *n = alloc_node(NODE_PROGRAM);
    n->data.list.count = 1;
    n->data.list.capacity = 1;
    n->data.list.children = (AST **)malloc(sizeof(AST *));
    if (!n->data.list.children) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }
    n->data.list.children[0] = stmt_list;
    return n;
}

AST *create_var_decl(char *name, AST *init) {
    AST *n = alloc_node(NODE_VAR_DECL);
    n->data.var_decl.name = strdup(name);
    n->data.var_decl.initializer = init;
    return n;
}

AST *create_assignment(char *name, AST *expr) {
    AST *n = alloc_node(NODE_ASSIGNMENT);
    n->data.assignment.name = strdup(name);
    n->data.assignment.expr = expr;
    return n;
}

AST *create_if_stmt(AST *cond, AST *then_s, AST *else_s) {
    AST *n = alloc_node(NODE_IF);
    n->data.if_stmt.condition = cond;
    n->data.if_stmt.then_branch = then_s;
    n->data.if_stmt.else_branch = else_s;
    return n;
}

AST *create_while_stmt(AST *cond, AST *body) {
    AST *n = alloc_node(NODE_WHILE);
    n->data.while_stmt.condition = cond;
    n->data.while_stmt.body = body;
    return n;
}

AST *create_block(AST *stmt_list) {
    AST *n = alloc_node(NODE_BLOCK);
    n->data.list.count = 1;
    n->data.list.capacity = 1;
    n->data.list.children = (AST **)malloc(sizeof(AST *));
    if (!n->data.list.children) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }
    n->data.list.children[0] = stmt_list;
    return n;
}

AST *create_bin_op(BinOp op, AST *l, AST *r) {
    AST *n = alloc_node(NODE_BIN_OP);
    n->data.bin_op.op = op;
    n->data.bin_op.left = l;
    n->data.bin_op.right = r;
    return n;
}

AST *create_unary_op(UnaryOp op, AST *e) {
    AST *n = alloc_node(NODE_UNARY_OP);
    n->data.unary_op.op = op;
    n->data.unary_op.expr = e;
    return n;
}

AST *create_integer(int v) {
    AST *n = alloc_node(NODE_INTEGER);
    n->data.int_value = v;
    return n;
}

AST *create_identifier(char *name) {
    AST *n = alloc_node(NODE_IDENTIFIER);
    n->data.identifier = strdup(name);
    return n;
}

static void indent_print(int k) {
    while (k-- > 0) printf("  ");
}

static const char *binop_str(BinOp op) {
    switch (op) {
        case OP_PLUS: return "+";
        case OP_MINUS: return "-";
        case OP_MUL: return "*";
        case OP_DIV: return "/";
        case OP_EQ: return "==";
        case OP_NE: return "!=";
        case OP_LT: return "<";
        case OP_GT: return ">";
        case OP_LE: return "<=";
        case OP_GE: return ">=";
        default: return "?";
    }
}

void print_ast(AST *n, int ind) {
    if (!n) return;

    switch (n->type) {
        case NODE_PROGRAM:
            indent_print(ind); printf("PROGRAM\n");
            for (int i = 0; i < n->data.list.count; ++i)
                print_ast(n->data.list.children[i], ind + 1);
            break;
        case NODE_STMT_LIST:
            indent_print(ind); printf("STMT_LIST (%d)\n", n->data.list.count);
            for (int i = 0; i < n->data.list.count; ++i)
                print_ast(n->data.list.children[i], ind + 1);
            break;
        case NODE_VAR_DECL:
            indent_print(ind); printf("VAR_DECL %s\n", n->data.var_decl.name);
            if (n->data.var_decl.initializer) {
                indent_print(ind + 1); printf("initializer:\n");
                print_ast(n->data.var_decl.initializer, ind + 2);
            }
            break;
        case NODE_ASSIGNMENT:
            indent_print(ind); printf("ASSIGN %s\n", n->data.assignment.name);
            print_ast(n->data.assignment.expr, ind + 1);
            break;
        case NODE_IF:
            indent_print(ind); printf("IF\n");
            indent_print(ind + 1); printf("cond:\n");
            print_ast(n->data.if_stmt.condition, ind + 2);
            indent_print(ind + 1); printf("then:\n");
            print_ast(n->data.if_stmt.then_branch, ind + 2);
            if (n->data.if_stmt.else_branch) {
                indent_print(ind + 1); printf("else:\n");
                print_ast(n->data.if_stmt.else_branch, ind + 2);
            }
            break;
        case NODE_WHILE:
            indent_print(ind); printf("WHILE\n");
            indent_print(ind + 1); printf("cond:\n");
            print_ast(n->data.while_stmt.condition, ind + 2);
            indent_print(ind + 1); printf("body:\n");
            print_ast(n->data.while_stmt.body, ind + 2);
            break;
        case NODE_BLOCK:
            indent_print(ind); printf("BLOCK\n");
            for (int i = 0; i < n->data.list.count; ++i)
                print_ast(n->data.list.children[i], ind + 1);
            break;
        case NODE_BIN_OP:
            indent_print(ind); printf("BIN_OP %s\n", binop_str(n->data.bin_op.op));
            print_ast(n->data.bin_op.left, ind + 1);
            print_ast(n->data.bin_op.right, ind + 1);
            break;
        case NODE_UNARY_OP:
            indent_print(ind); printf("UNARY_OP %s\n",
                                     n->data.unary_op.op == UNARY_PLUS ? "+" : "-");
            print_ast(n->data.unary_op.expr, ind + 1);
            break;
        case NODE_INTEGER:
            indent_print(ind); printf("INT %d\n", n->data.int_value);
            break;
        case NODE_IDENTIFIER:
            indent_print(ind); printf("ID %s\n", n->data.identifier);
            break;
    }
}

void free_ast(AST *n) {
    if (!n) return;

    switch (n->type) {
        case NODE_PROGRAM:
        case NODE_STMT_LIST:
        case NODE_BLOCK:
            for (int i = 0; i < n->data.list.count; ++i)
                free_ast(n->data.list.children[i]);
            free(n->data.list.children);
            break;
        case NODE_VAR_DECL:
            free(n->data.var_decl.name);
            free_ast(n->data.var_decl.initializer);
            break;
        case NODE_ASSIGNMENT:
            free(n->data.assignment.name);
            free_ast(n->data.assignment.expr);
            break;
        case NODE_IF:
            free_ast(n->data.if_stmt.condition);
            free_ast(n->data.if_stmt.then_branch);
            free_ast(n->data.if_stmt.else_branch);
            break;
        case NODE_WHILE:
            free_ast(n->data.while_stmt.condition);
            free_ast(n->data.while_stmt.body);
            break;
        case NODE_BIN_OP:
            free_ast(n->data.bin_op.left);
            free_ast(n->data.bin_op.right);
            break;
        case NODE_UNARY_OP:
            free_ast(n->data.unary_op.expr);
            break;
        case NODE_IDENTIFIER:
            free(n->data.identifier);
            break;
        case NODE_INTEGER:
            break;
    }
    free(n);
}
