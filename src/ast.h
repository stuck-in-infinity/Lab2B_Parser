#ifndef AST_H
#define AST_H

typedef enum
{
  NODE_PROGRAM,
  NODE_BLOCK,
  NODE_VAR_DECL,
  NODE_ASSIGN,
  NODE_IF,
  NODE_WHILE,
  NODE_BINOP,
  NODE_NUMBER,
  NODE_IDENT
} NodeType;

typedef struct ASTNode
{
  NodeType type;
  char *name; // variable / operator
  int value;  // numbers
  struct ASTNode *left;
  struct ASTNode *right;
  struct ASTNode *cond;
  struct ASTNode *body;
  struct ASTNode *else_body;
  struct ASTNode *next; // for statement lists
} ASTNode;

ASTNode *make_node(NodeType type);
ASTNode *make_number(int v);
ASTNode *make_ident(char *s);
ASTNode *make_binop(char *op, ASTNode *l, ASTNode *r);
void print_ast(ASTNode *node, int indent);

#endif
