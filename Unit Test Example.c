void test_symbol_table() {
    add_symbol("x", "int");
    struct Symbol* symbol = find_symbol("x");
    assert(symbol != NULL);
    assert(strcmp(symbol->type, "int") == 0);
}
