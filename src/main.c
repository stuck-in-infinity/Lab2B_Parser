#include <stdio.h>
#include "ast.h"

extern ASTNode *root;
int yyparse();

int main()
{
  printf("Starting parser...\n");
  if (yyparse() == 0)
  {
    printf("Parsing successful\n");
    print_ast(root, 0);
  }
  else
  {
    printf("Parsing failed\n");
  }
  return 0;
}
