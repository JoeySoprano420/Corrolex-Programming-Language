typedef struct {
    char *source_code;
    CorrolexIR *ir;
    Bytecode *bytecode;
} CorrolexScript;

void compile(CorrolexScript *script) {
    script->ir = parse_to_ir(script->source_code);
    optimize_ir(script->ir);
    script->bytecode = generate_bytecode(script->ir);
}
