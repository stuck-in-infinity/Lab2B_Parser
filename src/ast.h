#ifndef AST_H
#define AST_H

typedef enum
{
  NODE_VAR_DECL,
  NODE_ASSIGN,
  NODE_BINOP,
  NODE_NUMBER,
  NODE_IDENT
} NodeType;

typedef struct ASTNode
{
  NodeType type;
  char *name;
  int value;
  char op;
  struct ASTNode *left;
  struct ASTNode *right;
} ASTNode;

ASTNode *create_var_decl(char *name, ASTNode *value);
ASTNode *create_assign(char *name, ASTNode *value);
ASTNode *create_binop(char op, ASTNode *left, ASTNode *right);
ASTNode *create_number(int value);
ASTNode *create_ident(char *name);

void print_ast(ASTNode *node, int indent);

#endif
