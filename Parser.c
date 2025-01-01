%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokenizer.c"
#include "symbol_table.c"
#include "optimizer.c" // New optimizer for compile-time enhancements
#include "error_handler.c" // Enhanced error tracking and isolation

extern int yylex();
extern FILE *yyin;

void yyerror(char *s) {
    fprintf(stderr, "Syntax error: %s\n", s);
}

%}

%token IDENTIFIER NUMBER OPERATOR KEYWORD PUNCTUATION COMMENT
%token RETURN IF ELSE WHILE FOR BREAK CONTINUE VOID ASYNC AWAIT
%token STATIC THREADSAFE OWNED BORROWED

%%

program:
    declaration_list
;

declaration_list:
    declaration
    | declaration_list declaration
;

declaration:
    variable_declaration
    | function_declaration
    | async_function_declaration // New for async-first paradigm
;

variable_declaration:
    type IDENTIFIER ';'
    | type IDENTIFIER '=' expression ';'
    | STATIC type IDENTIFIER ';'
    | THREADSAFE type IDENTIFIER ';'
;

function_declaration:
    type IDENTIFIER '(' parameter_list ')' compound_statement
    | VOID IDENTIFIER '(' parameter_list ')' compound_statement
;

async_function_declaration:
    ASYNC type IDENTIFIER '(' parameter_list ')' compound_statement
    | ASYNC VOID IDENTIFIER '(' parameter_list ')' compound_statement
;

parameter_list:
    /* Empty */
    | parameter_declaration
    | parameter_list ',' parameter_declaration
;

parameter_declaration:
    type IDENTIFIER
    | BORROWED type IDENTIFIER
    | OWNED type IDENTIFIER
;

type:
    "int" { $$ = strdup("int"); }
    | "float" { $$ = strdup("float"); }
    | "char" { $$ = strdup("char"); }
    | "bool" { $$ = strdup("bool"); }
;

compound_statement:
    '{' statement_list '}'
;

statement_list:
    /* Empty */
    | statement
    | statement_list statement
;

statement:
    variable_declaration
    | expression_statement
    | compound_statement
    | selection_statement
    | iteration_statement
    | jump_statement
    | await_statement // New for async/await
;

await_statement:
    AWAIT expression ';'
;

selection_statement:
    IF '(' expression ')' statement
    | IF '(' expression ')' statement ELSE statement
;

iteration_statement:
    WHILE '(' expression ')' statement
    | FOR '(' expression_statement expression_statement expression ')' statement
;

jump_statement:
    RETURN expression ';'
    | RETURN ';'
    | BREAK ';'
    | CONTINUE ';'
;

expression_statement:
    expression ';'
;

expression:
    IDENTIFIER
    | NUMBER
    | expression OPERATOR expression
    | IDENTIFIER '[' expression ']'
    | '(' expression ')'
    | expression '?' expression ':' expression
;

%%

int main() {
    printf("Enter optimized Corrolex code:\n");
    yyparse();
    return 0;
}
