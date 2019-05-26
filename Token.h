#ifndef TOKEN_H
#define TOKEN_H

#include "Value.h"

#include <iostream>
#include <string>
#include <vector>

enum TokenType {
    Eof = -1,
    TScalar,
    TOperator,
    TLBracket,
    TRBracket,
    TLParen,
    TRParen
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
        case TScalar:
            return "Scalar";
        case TOperator:
            return "Operator";
        case TLBracket:
            return "LBracket";
        case TRBracket:
            return "RBracket";
        case TLParen:
            return "LParen";
        case TRParen:
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