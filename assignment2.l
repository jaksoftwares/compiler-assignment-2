%{
    #include "assignment2.tab.h"
%}

%%

[a]     { yylval='a'; return a; }
[b]     { yylval='b'; return b; }
\n      { return NL; }
[ \t]   { /* ignore whitespace */ }
.       { return yytext[0]; }

%%

int yywrap()
{
    return 1;
}