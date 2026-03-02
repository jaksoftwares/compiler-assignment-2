%{
    #include<stdio.h>
    int yylex();
    void yyerror(const char *s);
%}

%token NAME NUMBER

%%

statement: NAME '=' expression
         | expression { printf("=%d\n", $1); }
         ;

expression: expression '+' NUMBER { $$ = $1 + $3; }
          | expression '-' NUMBER { $$ = $1 - $3; }
          | NUMBER                { $$ = $1; }
          ;

%%

void yyerror(const char *s)
{
    printf("syntax error\n");
}

int main()
{
    yyparse();
    return 0;
}