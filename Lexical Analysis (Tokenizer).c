#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_SIZE 256

enum TokenType {
    KEYWORD,
    IDENTIFIER,
    NUMBER,
    OPERATOR,
    END,
    ERROR
};

struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_SIZE];
};

struct Token get_next_token(FILE *file) {
    char ch;
    struct Token token;
    token.value[0] = '\0';

    ch = fgetc(file);
    while (isspace(ch)) {
        ch = fgetc(file);
    }

    if (ch == EOF) {
        token.type = END;
        return token;
    }

    if (isalpha(ch)) {
        token.type = IDENTIFIER;
        int i = 0;
        while (isalnum(ch)) {
            token.value[i++] = ch;
            ch = fgetc(file);
        }
        token.value[i] = '\0';
        return token;
    }

    if (isdigit(ch)) {
        token.type = NUMBER;
        int i = 0;
        while (isdigit(ch)) {
            token.value[i++] = ch;
            ch = fgetc(file);
        }
        token.value[i] = '\0';
        return token;
    }

    if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
        token.type = OPERATOR;
        token.value[0] = ch;
        token.value[1] = '\0';
        return token;
    }

    token.type = ERROR;
    return token;
}
