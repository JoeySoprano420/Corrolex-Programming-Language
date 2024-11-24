struct ASTNode {
    enum { VAR, CONST, OP } type;
    union {
        char *var_name;
        int value;
        struct { struct ASTNode *left; struct ASTNode *right; char op; };
    };
};

void generate_code(struct ASTNode *node) {
    if (node->type == CONST) {
        printf("PUSH %d\n", node->value);
    } else if (node->type == VAR) {
        printf("LOAD %s\n", node->var_name);
    } else if (node->type == OP) {
        generate_code(node->left);
        generate_code(node->right);
        printf("OPERATE %c\n", node->op);
    }
}

// Representation of an IR instruction
struct IRInstruction {
    char op[20];  // Operation (e.g., "assign", "add")
    char operand1[256];  // Operand 1
    char operand2[256];  // Operand 2 (if applicable)
    char result[256];  // Result (if applicable)
};
