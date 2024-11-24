// Code generation for declarations
void generate_declaration_code(const char* name, const char* type) {
    printf("Generated IR: Declare %s of type %s\n", name, type);
}

// Code generation for assignments
void generate_assignment_code(const char* name, const char* value) {
    printf("Generated IR: Assign %s = %s\n", name, value);
}

// Code generation for expressions
void generate_expression_code(const char* op, const char* operand1, const char* operand2, const char* result) {
    printf("Generated IR: %s %s, %s -> %s\n", op, operand1, operand2, result);
}

// Code generation for declarations
void generate_declaration_code(const char* name, const char* type) {
    printf("Generated IR: Declare %s of type %s\n", name, type);
}

// Code generation for assignments
void generate_assignment_code(const char* name, const char* value) {
    printf("Generated IR: Assign %s = %s\n", name, value);
}

// Code generation for expressions
void generate_expression_code(const char* op, const char* operand1, const char* operand2, const char* result) {
    printf("Generated IR: %s %s, %s -> %s\n", op, operand1, operand2, result);
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

