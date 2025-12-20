%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"

int line_num = 1;
%}

%option noyywrap
%option yylineno

digit       [0-9]
letter      [a-zA-Z_]
identifier  {letter}({letter}|{digit})*
integer     {digit}+
ws          [ \t\r]+
nl          \n

%%

{nl}                { line_num++; }
{ws}                { /* skip */ }

/* single-line comments */
"//".*              { /* skip */ }

/* multi-line comments (non-nested, simple) */
"/*"                {
                       int c, prev = 0;
                       while ((c = input()) != EOF) {
                           if (c == '\n') line_num++;
                           if (prev == '*' && c == '/') break;
                           prev = c;
                       }
                    }

/* keywords (must be before identifier) */
"var"               { return VAR; }
"if"                { return IF; }
"else"              { return ELSE; }
"while"             { return WHILE; }

/* operators and delimiters */
"=="                { return EQ; }
"!="                { return NE; }
"<="                { return LE; }
">="                { return GE; }
"<"                 { return LT; }
">"                 { return GT; }
"="                 { return ASSIGN; }
"+"                 { return PLUS; }
"-"                 { return MINUS; }
"*"                 { return MUL; }
"/"                 { return DIV; }
"("                 { return LPAREN; }
")"                 { return RPAREN; }
"{"                 { return LBRACE; }
"}"                 { return RBRACE; }
";"                 { return SEMICOLON; }

/* literals and identifiers */
{integer}           {
                        yylval.int_val = atoi(yytext);
                        return INTEGER;
                    }

{identifier}        {
                        yylval.string_val = strdup(yytext);
                        return IDENTIFIER;
                    }

/* catch-all for bad characters */
.                   {
                        fprintf(stderr,
                                "Lexical error at line %d: unexpected character '%s'\n",
                                line_num, yytext);
                    }

%%

int yywrap(void) { return 1; }
