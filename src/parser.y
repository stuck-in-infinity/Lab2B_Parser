%code requires {
    #include "ast.h"
}

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

ASTNode *root;
int yylex();
void yyerror(const char *s);
extern int yylineno;
%}

%union {
    int ival;
    char *sval;
    ASTNode *node;
}

%token <ival> NUMBER
%token <sval> IDENT
%token VAR IF ELSE WHILE
%token EQ NE LT GT LE GE

%type <node> program stmt stmt_list block expr equality comparison term factor unary primary

%left EQ NE
%left LT GT LE GE
%left '+' '-'
%left '*' '/'
%right UMINUS

%%

program:
    stmt_list {
        root = make_node(NODE_PROGRAM);
        root->body = $1;
    }
;

stmt_list:
      /* empty */        { $$ = NULL; }
    | stmt               { $$ = $1; }
    | stmt_list stmt     {
          if ($1 == NULL) {
              $$ = $2;
          } else {
              ASTNode *n = $1;
              while (n->next != NULL)
                  n = n->next;
              n->next = $2;
              $$ = $1;
          }
      }
;

stmt:
      VAR IDENT ';'
        {
            $$ = make_node(NODE_VAR_DECL);
            $$->name = $2;
            $$->right = NULL;
        }
    | VAR IDENT '=' expr ';'
        {
            $$ = make_node(NODE_VAR_DECL);
            $$->name = $2;
            $$->right = $4;
        }
    | IDENT '=' expr ';'
        {
            $$ = make_node(NODE_ASSIGN);
            $$->name = $1;
            $$->right = $3;
        }
    | IF '(' expr ')' stmt ELSE stmt
        {
            $$ = make_node(NODE_IF);
            $$->cond = $3;
            $$->body = $5;
            $$->else_body = $7;
        }
    | IF '(' expr ')' stmt
        {
            $$ = make_node(NODE_IF);
            $$->cond = $3;
            $$->body = $5;
            $$->else_body = NULL;
        }
    | WHILE '(' expr ')' stmt
        {
            $$ = make_node(NODE_WHILE);
            $$->cond = $3;
            $$->body = $5;
        }
    | block
;

block:
      '{' stmt_list '}'
        {
            $$ = make_node(NODE_BLOCK);
            $$->body = $2;
        }
    | '{' '}'
        {
            $$ = make_node(NODE_BLOCK);
            $$->body = NULL;
        }
;

expr:
    equality { $$ = $1; }
;

equality:
      comparison           { $$ = $1; }
    | equality EQ comparison
        { $$ = make_binop("==", $1, $3); }
    | equality NE comparison
        { $$ = make_binop("!=", $1, $3); }
;

comparison:
      term                 { $$ = $1; }
    | comparison LT term   { $$ = make_binop("<",  $1, $3); }
    | comparison GT term   { $$ = make_binop(">",  $1, $3); }
    | comparison LE term   { $$ = make_binop("<=", $1, $3); }
    | comparison GE term   { $$ = make_binop(">=", $1, $3); }
;

term:
      factor               { $$ = $1; }
    | term '+' factor      { $$ = make_binop("+", $1, $3); }
    | term '-' factor      { $$ = make_binop("-", $1, $3); }
;

factor:
      unary                { $$ = $1; }
    | factor '*' unary     { $$ = make_binop("*", $1, $3); }
    | factor '/' unary     { $$ = make_binop("/", $1, $3); }
;

unary:
      primary              { $$ = $1; }
    | '-' unary %prec UMINUS
        { $$ = make_binop("-", NULL, $2); }
    | '+' unary
        { $$ = $2; }
;

primary:
      NUMBER
        { $$ = make_number($1); }
    | IDENT
        { $$ = make_ident($1); }
    | '(' expr ')'
        { $$ = $2; }
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error at line %d: %s\n", yylineno, s);
}
