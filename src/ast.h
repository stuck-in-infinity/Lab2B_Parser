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
  char *name; // variable name
  int value;  // for numbers
  char op;    // operator
  struct ASTNode *left;
  struct ASTNode *right;
} ASTNode;

/* Constructors */
ASTNode *make_var_decl(char *name, ASTNode *value);
ASTNode *make_assign(char *name, ASTNode *value);
ASTNode *make_binop(char op, ASTNode *left, ASTNode *right);
ASTNode *make_number(int value);
ASTNode *make_ident(char *name);

/* Print */
void print_ast(ASTNode *node, int indent);

#endif
