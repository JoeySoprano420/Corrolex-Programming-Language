%{
#include <stdio.h>
#include <stdlib.h>

int yylex();
void yyerror(char *s);
%}

%token IDENTIFIER NUMBER OPERATOR
%start program

%%

program:
    statement_list
;

statement_list:
    statement
    | statement_list statement
;

statement:
    IDENTIFIER '=' expression ';'
    | expression ';'
;

expression:
    IDENTIFIER
    | NUMBER
    | expression OPERATOR expression
;

%%

void yyerror(char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main() {
    yyparse();
    return 0;
}
