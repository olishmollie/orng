#ifndef PARSER_H
#define PARSER_H

#include "Ast.h"
#include "Error.h"
#include "Lexer.h"

class ParseError : public OrngError {
    std::string msg;
    unsigned int column;

  public:
    ParseError(std::string msg, unsigned int column);
    std::string caused_by();
};

class Parser {
    Lexer lexer;
    Token curtok;
    Token peektok;

    void next();
    Token peek();

    Number parse_integer();
    Number parse_real();
    Number parse_complex();

    Ast *expr(bool paren_ok);
    Ast *operand(bool paren_ok);
    Ast *atom();
    Ast *vector();

  public:
    Parser(char *source);

    Ast *parse();
};

#endif