#ifndef PARSER_H
#define PARSER_H

#include "Ast.h"
#include "Error.h"
#include "Lexer.h"

class ParseError : public OrngError {
    std::string msg;
    std::string source;
    unsigned int column;

  public:
    ParseError(std::string msg, std::string source, unsigned int column);
    std::string caused_by();
};

class Parser {
    std::string source;

    Lexer lexer;
    Token curtok;
    Token peektok;

    void next();
    Token peek();

    Number parse_integer();
    Number parse_real();
    Number parse_complex();

    Expr *expr(bool paren_ok);
    Expr *operand(bool paren_ok);
    Expr *atom();
    Expr *vector();

  public:
    Parser(char *source);

    Ast parse();
};

#endif