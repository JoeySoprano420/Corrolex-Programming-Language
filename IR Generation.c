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
