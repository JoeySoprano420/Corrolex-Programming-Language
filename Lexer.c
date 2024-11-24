#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_TOKEN_SIZE 256

// Token Types
enum TokenType {
    KEYWORD, IDENTIFIER, NUMBER, OPERATOR, COMMENT, PUNCTUATION, END, ERROR
};

// Token structure
struct Token {
    enum TokenType type;
    char value[MAX_TOKEN_SIZE];
};

// Keyword list
const char *keywords[] = {"if", "else", "while", "return", "int", "float", "char"};
const int num_keywords = 7; // Number of keywords

// Function to check if a string is a keyword
int is_keyword(const char *str) {
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Function to get the next token
struct Token get_next_token(FILE *file) {
    char ch;
    struct Token token;
    token.value[0] = '\0';  // Clear token value

    ch = fgetc(file);
    while (isspace(ch)) {
        ch = fgetc(file);  // Skip white spaces
    }

    // End of file
    if (ch == EOF) {
        token.type = END;
        return token;
    }

    // Handle comments (single-line comments)
    if (ch == '/') {
        ch = fgetc(file);
        if (ch == '/') {  // Single-line comment
            while (ch != '\n' && ch != EOF) {
                ch = fgetc(file);
            }
            token.type = COMMENT;
            strcpy(token.value, "Comment");
            return token;
        } else {
            // Handle as operator
            ungetc(ch, file);
            token.type = OPERATOR;
            token.value[0] = '/';
            token.value[1] = '\0';
            return token;
        }
    }

    // Handle identifiers and keywords
    if (isalpha(ch) || ch == '_') {
        int i = 0;
        while (isalnum(ch) || ch == '_') {
            token.value[i++] = ch;
            ch = fgetc(file);
        }
        token.value[i] = '\0';

        if (is_keyword(token.value)) {
            token.type = KEYWORD;
        } else {
            token.type = IDENTIFIER;
        }
        ungetc(ch, file);
        return token;
    }

    // Handle numbers
    if (isdigit(ch)) {
        int i = 0;
        while (isdigit(ch)) {
            token.value[i++] = ch;
            ch = fgetc(file);
        }
        token.value[i] = '\0';
        token.type = NUMBER;
        ungetc(ch, file);
        return token;
    }

    // Handle operators
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=' || ch == '<' || ch == '>') {
        token.type = OPERATOR;
        token.value[0] = ch;
        token.value[1] = '\0';
        return token;
    }

    // Handle punctuation (semicolon, parentheses, etc.)
    if (ch == ';' || ch == '(' || ch == ')' || ch == '{' || ch == '}') {
        token.type = PUNCTUATION;
        token.value[0] = ch;
        token.value[1] = '\0';
        return token;
    }

    // Error handling for unexpected characters
    token.type = ERROR;
    return token;
}
