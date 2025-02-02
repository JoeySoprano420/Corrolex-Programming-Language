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

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokenizer.c"
#include "symbol_table.c"  // Include the symbol table

extern int yylex();  // Declare the lexer function
extern FILE *yyin;   // Declare the input file stream

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
    type IDENTIFIER ';' { 
        if (find_symbol($2)) {
            printf("Error: Variable '%s' already declared.\n", $2);
        } else {
            add_symbol($2, $1);
            printf("Declared variable: %s of type %s\n", $2, $1);
        }
    }
    | type IDENTIFIER '=' expression ';' { 
        if (find_symbol($2)) {
            printf("Error: Variable '%s' already declared.\n", $2);
        } else {
            add_symbol($2, $1);
            printf("Declared and initialized variable: %s of type %s\n", $2, $1);
        }
    }
;

type:
    "int" { strcpy($$, "int"); }
    | "float" { strcpy($$, "float"); }
    | "char" { strcpy($$, "char"); }
;

expression:
    IDENTIFIER { 
        if (!find_symbol($1)) {
            printf("Error: Variable '%s' not declared.\n", $1);
        }
    }
    | NUMBER { /* No semantic error for numbers */ }
    | expression OPERATOR expression { 
        // Simple type-checking (e.g., no mixed types for operations)
        if (strcmp($1, $3) != 0) {
            printf("Error: Type mismatch in operation.\n");
        } else {
            $$ = $1; // Propagate the type from the operands
        }
    }
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

declaration:
    type IDENTIFIER ';' { 
        add_symbol($2, $1);
        generate_declaration_code($2, $1);
    }
    | type IDENTIFIER '=' expression ';' { 
        add_symbol($2, $1);
        generate_assignment_code($2, $4);
        generate_expression_code("assign", $2, $4, $2);
    }
;

expression:
    IDENTIFIER { 
        if (!find_symbol($1)) {
            printf("Error: Variable '%s' not declared.\n", $1);
        }
        $$ = $1; // Propagate the variable name
    }
    | NUMBER { $$ = $1; }  // Numbers are passed as-is
    | expression OPERATOR expression { 
        $$ = $1; // For simplicity, assume types match
        generate_expression_code("op", $1, $3, $1);
    }
;

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

