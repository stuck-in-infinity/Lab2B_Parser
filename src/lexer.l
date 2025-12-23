%{
#include "parser.tab.h"
#include <stdlib.h>
#include <string.h>
%}

%%
"var"           { return VAR; }
[0-9]+          { yylval.num = atoi(yytext); return NUMBER; }
[a-zA-Z_][a-zA-Z0-9_]* {
                    yylval.id = strdup(yytext);
                    return IDENT;
                 }

"="             { return '='; }
";"             { return ';'; }
"+"|"-"|"*"|"/" { return yytext[0]; }

[ \t\n]+        ;
.               ;
%%

int yywrap()
{
    return 1;
}
