int main() {
    FILE *file = fopen("test.cor", "r");
    if (!file) {
        printf("Unable to open file.\n");
        return 1;
    }

    struct Token token;
    while ((token = get_next_token(file)).type != END) {
        printf("Token: %s, Type: %d\n", token.value, token.type);
    }

    fclose(file);
    return 0;
}
