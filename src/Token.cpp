#include "Token.h"

#include <sstream>

Token::Token(TokenType typ, std::string lex, int pos)
    : type(typ), lexeme(lex), position(pos) {}

std::string Token::type_name() const {
    switch (type) {
    case Eof:
        return "Eof";
    case TokInteger:
        return "Scalar";
    case TokComplex:
        return "Complex";
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

std::string Token::to_string() const {
    std::ostringstream os;
    os << this;
    return os.str();
}

std::ostream &operator<<(std::ostream &os, const Token &t) {
    switch (t.type) {
    case Eof:
        os << "<Token{type: " << t.type_name() << ", position: " << t.position
           << "}>";
        break;
    default:
        os << "<Token{type: " << t.type_name() << ", lexeme: '" << t.lexeme
           << "', position: " << t.position << "}>";
        break;
    }
    return os;
}
