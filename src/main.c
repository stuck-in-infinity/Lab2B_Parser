#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

extern int yyparse(void);
extern FILE *yyin;
extern AST *root;

int main(int argc, char **argv) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror("fopen");
            return 1;
        }
    } else {
        yyin = stdin;
    }

    if (yyparse() == 0) {
        printf("=== Parse successful ===\n");
        if (root) {
            print_ast(root, 0);
            free_ast(root);
        }
    } else {
        fprintf(stderr, "=== Parse failed ===\n");
    }

    if (yyin != stdin) fclose(yyin);
    return 0;
}
