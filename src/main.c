#include <stdio.h>

int yyparse(void);

int main()
{
  printf("Starting parser...\n");

  if (yyparse() == 0)
    printf("Parsing successful\n");
  else
    printf("Parsing failed\n");

  return 0;
}

