// Lexer: Tokenizer for Corrolex Syntax
pub enum Token {
    Keyword(String),
    Identifier(String),
    Operator(String),
    Literal(String),
    Separator(char),
}

pub fn lex(input: &str) -> Vec<Token> {
    let mut tokens = Vec::new();
    let words = input.split_whitespace();

    for word in words {
        if is_keyword(word) {
            tokens.push(Token::Keyword(word.to_string()));
        } else if is_operator(word) {
            tokens.push(Token::Operator(word.to_string()));
        } else if is_literal(word) {
            tokens.push(Token::Literal(word.to_string()));
        } else {
            tokens.push(Token::Identifier(word.to_string()));
        }
    }
    tokens
}

fn is_keyword(word: &str) -> bool {
    matches!(word, "module" | "function" | "let" | "return" | "if" | "case" | "use")
}

fn is_operator(word: &str) -> bool {
    matches!(word, "=" | "=>" | ">" | "<" | "+=" | "-=")
}

fn is_literal(word: &str) -> bool {
    word.chars().all(|c| c.is_numeric() || c == '.')
}
