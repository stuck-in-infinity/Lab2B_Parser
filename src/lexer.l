%{
#include "parser.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
%}

%option yylineno

%%

"var"       { return VAR; }
"if"        { return IF; }
"else"      { return ELSE; }
"while"     { return WHILE; }

"=="        { return EQ; }
"!="        { return NE; }
"<="        { return LE; }
">="        { return GE; }
"<"         { return LT; }
">"         { return GT; }

"+"         { return '+'; }
"-"         { return '-'; }
"*"         { return '*'; }
"/"         { return '/'; }
"="         { return '='; }
";"         { return ';'; }
"("         { return '('; }
")"         { return ')'; }
"{"         { return '{'; }
"}"         { return '}'; }

"//"[^\n]*  { /* skip single-line comment */ }

"/*"        { 
                int c;
                while ((c = input()) != 0) {
                    if (c == '*') {
                        if ((c = input()) == '/') break;
                        else unput(c);
                    }
                }
            }

[0-9]+      { 
                yylval.ival = atoi(yytext); 
                return NUMBER; 
            }

[a-zA-Z_][a-zA-Z0-9_]*  { 
                            yylval.sval = strdup(yytext); 
                            return IDENT; 
                        }

[ \t\r\n]+ { }

.           { }

%%

int yywrap(void) { 
    return 1; 
}
