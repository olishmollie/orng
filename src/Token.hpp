#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>

enum TokenType {
    Eof = -1,
    TokIdentifier,
    TokInteger,
    TokReal,
    TokComplex,
    TokOperator,
    TokLeftBracket,
    TokRightBracket,
    TokLeftParen,
    TokRightParen
};

struct Token {
    TokenType type;
    int position;
    std::string lexeme;

    Token(TokenType typ, std::string lex, int pos);

    std::string type_name() const;
    std::string to_string() const;

    friend std::ostream &operator<<(std::ostream &os, const Token &t);
};

#endif