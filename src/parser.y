%code requires {
    #include "ast.h"
}

%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

extern int yylex();
void yyerror(const char *s);

ASTNode *root;
%}

%union {
    int num;
    char *id;
    ASTNode *node;
}

%token <id> IDENT
%token <num> NUMBER
%token VAR

%type <node> stmt expr

%left '+' '-'
%left '*' '/'

%%

program:
    program stmt
    | stmt
    ;

stmt:
      VAR IDENT '=' expr ';'
        { root = make_var_decl($2, $4); print_ast(root, 0); }
    | IDENT '=' expr ';'
        { root = make_assign($1, $3); print_ast(root, 0); }
    ;

expr:
      expr '+' expr   { $$ = make_binop('+', $1, $3); }
    | expr '-' expr   { $$ = make_binop('-', $1, $3); }
    | expr '*' expr   { $$ = make_binop('*', $1, $3); }
    | expr '/' expr   { $$ = make_binop('/', $1, $3); }
    | NUMBER          { $$ = make_number($1); }
    | IDENT           { $$ = make_ident($1); }
    ;

%%

void yyerror(const char *s)
{
    fprintf(stderr, "Error: %s\n", s);
}
