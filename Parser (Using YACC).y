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

%{
#include <stdio.h>
#include <stdlib.h>
#include "tokenizer.c"  // Include the lexer
%}

%token IDENTIFIER NUMBER OPERATOR KEYWORD PUNCTUATION COMMENT

%%

program:
    declaration_list
;

declaration_list:
    declaration
    | declaration_list declaration
;

declaration:
    type IDENTIFIER ';'
    | type IDENTIFIER '=' expression ';'
;

type:
    KEYWORD
;

expression:
    IDENTIFIER
    | NUMBER
    | expression OPERATOR expression
;

%%

int main() {
    printf("Enter Corrolex code:\n");
    yyparse();
    return 0;
}

int yyerror(char *s) {
    fprintf(stderr, "Syntax error: %s\n", s);
    return 1;
}
