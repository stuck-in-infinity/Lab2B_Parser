#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

static ASTNode *new_node(NodeType type)
{
  ASTNode *n = malloc(sizeof(ASTNode));
  n->type = type;
  n->name = NULL;
  n->value = 0;
  n->op = 0;
  n->left = n->right = NULL;
  return n;
}

ASTNode *create_var_decl(char *name, ASTNode *value)
{
  ASTNode *n = new_node(NODE_VAR_DECL);
  n->name = strdup(name);
  n->left = value;
  return n;
}

ASTNode *create_assign(char *name, ASTNode *value)
{
  ASTNode *n = new_node(NODE_ASSIGN);
  n->name = strdup(name);
  n->left = value;
  return n;
}

ASTNode *create_binop(char op, ASTNode *left, ASTNode *right)
{
  ASTNode *n = new_node(NODE_BINOP);
  n->op = op;
  n->left = left;
  n->right = right;
  return n;
}

ASTNode *create_number(int value)
{
  ASTNode *n = new_node(NODE_NUMBER);
  n->value = value;
  return n;
}

ASTNode *create_ident(char *name)
{
  ASTNode *n = new_node(NODE_IDENT);
  n->name = strdup(name);
  return n;
}

void print_ast(ASTNode *node, int indent)
{
  if (!node)
    return;

  for (int i = 0; i < indent; i++)
    printf("  ");

  switch (node->type)
  {
  case NODE_VAR_DECL:
    printf("VAR_DECL(%s)\n", node->name);
    print_ast(node->left, indent + 1);
    break;
  case NODE_ASSIGN:
    printf("ASSIGN(%s)\n", node->name);
    print_ast(node->left, indent + 1);
    break;
  case NODE_BINOP:
    printf("BINOP(%c)\n", node->op);
    print_ast(node->left, indent + 1);
    print_ast(node->right, indent + 1);
    break;
  case NODE_NUMBER:
    printf("NUMBER(%d)\n", node->value);
    break;
  case NODE_IDENT:
    printf("IDENT(%s)\n", node->name);
    break;
  }
}
