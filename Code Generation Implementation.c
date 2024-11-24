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
