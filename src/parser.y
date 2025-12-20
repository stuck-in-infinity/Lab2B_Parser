%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

extern int yylex(void);
extern int yylineno;
extern char *yytext;

void yyerror(const char *s);

AST *root = NULL;
%}

%union {
    int int_val;
    char *string_val;
    AST *node;
}

/* tokens */
%token VAR IF ELSE WHILE
%token ASSIGN EQ NE LT GT LE GE
%token PLUS MINUS MUL DIV
%token LPAREN RPAREN LBRACE RBRACE SEMICOLON
%token <int_val> INTEGER
%token <string_val> IDENTIFIER

/* nonterminals */
%type <node> program statement_list statement
%type <node> variable_decl assignment if_statement while_statement block
%type <node> expression equality comparison term factor unary primary

%left EQ NE
%left LT GT LE GE
%left PLUS MINUS
%left MUL DIV
%right UMINUS

%start program

%%

program
    : statement_list
      {
          root = create_program($1);
          $$ = root;
      }
    ;

statement_list
    : /* empty */
      {
          $$ = create_stmt_list();
      }
    | statement_list statement
      {
          if ($2) stmt_list_append($1, $2);
          $$ = $1;
      }
    ;

statement
    : variable_decl           { $$ = $1; }
    | assignment              { $$ = $1; }
    | if_statement            { $$ = $1; }
    | while_statement         { $$ = $1; }
    | block                   { $$ = $1; }
    | error SEMICOLON
      {
          fprintf(stderr, "Recovering from syntax error near line %d\n", yylineno);
          yyerrok;
          $$ = NULL;
      }
    ;

variable_decl
    : VAR IDENTIFIER SEMICOLON
      {
          $$ = create_var_decl($2, NULL);
          free($2);
      }
    | VAR IDENTIFIER ASSIGN expression SEMICOLON
      {
          $$ = create_var_decl($2, $4);
          free($2);
      }
    ;

assignment
    : IDENTIFIER ASSIGN expression SEMICOLON
      {
          $$ = create_assignment($1, $3);
          free($1);
      }
    ;

if_statement
    : IF LPAREN expression RPAREN statement
      {
          $$ = create_if_stmt($3, $5, NULL);
      }
    | IF LPAREN expression RPAREN statement ELSE statement
      {
          $$ = create_if_stmt($3, $5, $7);
      }
    ;

while_statement
    : WHILE LPAREN expression RPAREN statement
      {
          $$ = create_while_stmt($3, $5);
      }
    ;

block
    : LBRACE statement_list RBRACE
      {
          $$ = create_block($2);
      }
    ;

expression
    : equality     { $$ = $1; }
    ;

equality
    : comparison                           { $$ = $1; }
    | equality EQ comparison              { $$ = create_bin_op(OP_EQ, $1, $3); }
    | equality NE comparison              { $$ = create_bin_op(OP_NE, $1, $3); }
    ;

comparison
    : term                                { $$ = $1; }
    | comparison LT term                  { $$ = create_bin_op(OP_LT, $1, $3); }
    | comparison GT term                  { $$ = create_bin_op(OP_GT, $1, $3); }
    | comparison LE term                  { $$ = create_bin_op(OP_LE, $1, $3); }
    | comparison GE term                  { $$ = create_bin_op(OP_GE, $1, $3); }
    ;

term
    : factor                              { $$ = $1; }
    | term PLUS factor                    { $$ = create_bin_op(OP_PLUS, $1, $3); }
    | term MINUS factor                   { $$ = create_bin_op(OP_MINUS, $1, $3); }
    ;

factor
    : unary                               { $$ = $1; }
    | factor MUL unary                    { $$ = create_bin_op(OP_MUL, $1, $3); }
    | factor DIV unary                    { $$ = create_bin_op(OP_DIV, $1, $3); }
    ;

unary
    : primary                             { $$ = $1; }
    | PLUS unary                          { $$ = create_unary_op(UNARY_PLUS, $2); }
    | MINUS unary %prec UMINUS           { $$ = create_unary_op(UNARY_MINUS, $2); }
    ;

primary
    : INTEGER                             { $$ = create_integer($1); }
    | IDENTIFIER                          { $$ = create_identifier($1); free($1); }
    | LPAREN expression RPAREN            { $$ = $2; }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Parse error at line %d: %s (near '%s')\n",
            yylineno, s, yytext);
}
