#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

ASTNode *make_node(NodeType type)
{
  ASTNode *n = calloc(1, sizeof(ASTNode));
  n->type = type;
  return n;
}

ASTNode *make_number(int v)
{
  ASTNode *n = make_node(NODE_NUMBER);
  n->value = v;
  return n;
}

ASTNode *make_ident(char *s)
{
  ASTNode *n = make_node(NODE_IDENT);
  n->name = strdup(s);
  return n;
}

ASTNode *make_binop(char *op, ASTNode *l, ASTNode *r)
{
  ASTNode *n = make_node(NODE_BINOP);
  n->name = strdup(op);
  n->left = l;
  n->right = r;
  return n;
}

static void indent(int n)
{
  for (int i = 0; i < n; i++)
    printf("  ");
}

void print_ast(ASTNode *n, int d)
{
  if (!n)
    return;

  indent(d);
  switch (n->type)
  {
  case NODE_PROGRAM:
    printf("PROGRAM\n");
    break;
  case NODE_BLOCK:
    printf("BLOCK\n");
    break;
  case NODE_VAR_DECL:
    printf("VAR_DECL(%s)\n", n->name);
    break;
  case NODE_ASSIGN:
    printf("ASSIGN(%s)\n", n->name);
    break;
  case NODE_IF:
    printf("IF\n");
    break;
  case NODE_WHILE:
    printf("WHILE\n");
    break;
  case NODE_BINOP:
    printf("BINOP(%s)\n", n->name);
    break;
  case NODE_NUMBER:
    printf("NUMBER(%d)\n", n->value);
    break;
  case NODE_IDENT:
    printf("IDENT(%s)\n", n->name);
    break;
  }

  print_ast(n->cond, d + 1);
  print_ast(n->left, d + 1);
  print_ast(n->right, d + 1);
  print_ast(n->body, d + 1);
  print_ast(n->else_body, d + 1);
  print_ast(n->next, d);
}
