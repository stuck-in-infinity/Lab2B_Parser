#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

extern ASTNode *root;
int yyparse();

int main() {
    printf("Starting parser...\n");
    
    int result = yyparse();
    
    if (result == 0) {
        printf("Parsing successful!\n\n");
        printf("Abstract Syntax Tree:\n");
        printf("\n");
        if (root) {
            print_ast(root, 0);
        } else {
            printf("(empty program)\n");
        }
    } else {
        printf("Parsing failed!\n");
        return 1;
    }
    
    return 0;
}
