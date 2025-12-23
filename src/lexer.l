%{
#include "parser.tab.h"
#include <stdlib.h>
#include <string.h>
%}

%%
"var"        return VAR;
"if"         return IF;
"else"       return ELSE;
"while"      return WHILE;
"=="         return EQ;
"<"          return LT;
">"          return GT;
"+"          return '+';
"-"          return '-';
"*"          return '*';
"/"          return '/';
"="          return '=';
";"          return ';';
"("          return '(';
")"          return ')';
"{"          return '{';
"}"          return '}';

[0-9]+       { yylval.ival = atoi(yytext); return NUMBER; }
[a-zA-Z_][a-zA-Z0-9_]* {
                yylval.sval = strdup(yytext);
                return IDENT;
             }

[ \t\r\n]+   ;
.            ;

%%

int yywrap() { return 1; }
