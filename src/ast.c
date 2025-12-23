#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

static void indent(int n)
{
  for (int i = 0; i < n; i++)
    printf("  ");
}

ASTNode *make_var_decl(char *name, ASTNode *value)
{
  ASTNode *n = malloc(sizeof(ASTNode));
  n->type = NODE_VAR_DECL;
  n->name = strdup(name);
  n->left = value;
  n->right = NULL;
  return n;
}

ASTNode *make_assign(char *name, ASTNode *value)
{
  ASTNode *n = malloc(sizeof(ASTNode));
  n->type = NODE_ASSIGN;
  n->name = strdup(name);
  n->left = value;
  n->right = NULL;
  return n;
}

ASTNode *make_binop(char op, ASTNode *left, ASTNode *right)
{
  ASTNode *n = malloc(sizeof(ASTNode));
  n->type = NODE_BINOP;
  n->op = op;
  n->left = left;
  n->right = right;
  return n;
}

ASTNode *make_number(int value)
{
  ASTNode *n = malloc(sizeof(ASTNode));
  n->type = NODE_NUMBER;
  n->value = value;
  n->left = n->right = NULL;
  return n;
}

ASTNode *make_ident(char *name)
{
  ASTNode *n = malloc(sizeof(ASTNode));
  n->type = NODE_IDENT;
  n->name = strdup(name);
  n->left = n->right = NULL;
  return n;
}

void print_ast(ASTNode *node, int indent_lvl)
{
  if (!node)
    return;

  indent(indent_lvl);

  switch (node->type)
  {
  case NODE_VAR_DECL:
    printf("VAR_DECL(%s)\n", node->name);
    print_ast(node->left, indent_lvl + 1);
    break;

  case NODE_ASSIGN:
    printf("ASSIGN(%s)\n", node->name);
    print_ast(node->left, indent_lvl + 1);
    break;

  case NODE_BINOP:
    printf("BINOP(%c)\n", node->op);
    print_ast(node->left, indent_lvl + 1);
    print_ast(node->right, indent_lvl + 1);
    break;

  case NODE_NUMBER:
    printf("NUMBER(%d)\n", node->value);
    break;

  case NODE_IDENT:
    printf("IDENT(%s)\n", node->name);
    break;
  }
}
