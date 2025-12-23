%code requires {
    #include "ast.h"
}

%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

ASTNode *root;
int yylex();
void yyerror(const char *s);
%}

%union {
    int ival;
    char *sval;
    ASTNode *node;
}

%token <ival> NUMBER
%token <sval> IDENT
%token VAR IF ELSE WHILE
%token EQ LT GT

%type <node> program stmt stmt_list block expr

%left EQ LT GT
%left '+' '-'
%left '*' '/'
%right UMINUS

%%
program:
    stmt_list { root = $1; }
;

stmt_list:
    stmt stmt_list { $1->next = $2; $$ = $1; }
  | stmt { $$ = $1; }
;

stmt:
    VAR IDENT '=' expr ';'
        { $$ = make_node(NODE_VAR_DECL); $$->name = $2; $$->right = $4; }
  | IDENT '=' expr ';'
        { $$ = make_node(NODE_ASSIGN); $$->name = $1; $$->right = $3; }
  | IF '(' expr ')' stmt ELSE stmt
        { $$ = make_node(NODE_IF); $$->cond = $3; $$->body = $5; $$->else_body = $7; }
  | IF '(' expr ')' stmt
        { $$ = make_node(NODE_IF); $$->cond = $3; $$->body = $5; }
  | WHILE '(' expr ')' stmt
        { $$ = make_node(NODE_WHILE); $$->cond = $3; $$->body = $5; }
  | block
;

block:
    '{' stmt_list '}'
        { $$ = make_node(NODE_BLOCK); $$->body = $2; }
;

expr:
    expr '+' expr { $$ = make_binop("+", $1, $3); }
  | expr '-' expr { $$ = make_binop("-", $1, $3); }
  | expr '*' expr { $$ = make_binop("*", $1, $3); }
  | expr '/' expr { $$ = make_binop("/", $1, $3); }
  | expr EQ expr  { $$ = make_binop("==", $1, $3); }
  | expr LT expr  { $$ = make_binop("<", $1, $3); }
  | expr GT expr  { $$ = make_binop(">", $1, $3); }
  | '(' expr ')'  { $$ = $2; }
  | NUMBER        { $$ = make_number($1); }
  | IDENT         { $$ = make_ident($1); }
;
%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error: %s\n", s);
}
