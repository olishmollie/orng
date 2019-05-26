#ifndef TOKEN_H
#define TOKEN_H

#include "Value.h"

#include <iostream>
#include <string>
#include <vector>

enum TokenType {
    Eof = -1,
    TokScalar,
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

    Token(TokenType typ, std::string lex, int pos)
        : type(typ), lexeme(lex), position(pos) {}

    std::string type_name() const {
        switch (type) {
        case Eof:
            return "Eof";
        case TokScalar:
            return "Scalar";
        case TokOperator:
            return "Operator";
        case TokLeftBracket:
            return "LBracket";
        case TokRightBracket:
            return "RBracket";
        case TokLeftParen:
            return "LParen";
        case TokRightParen:
            return "RParen";
        }
    }

    friend std::ostream &operator<<(std::ostream &os, const Token &t) {
        switch (t.type) {
        case Eof:
            os << "<Token type: " << t.type_name()
               << ", position: " << t.position << ">";
            break;
        default:
            os << "<Token type: " << t.type_name() << ", lexeme: '" << t.lexeme
               << "', position: " << t.position << ">";
            break;
        }
        return os;
    }
};

#endif