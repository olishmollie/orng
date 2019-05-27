#ifndef PARSER_H
#define PARSER_H

#include "Ast.h"
#include "Error.h"
#include "Lexer.h"

#include <complex>

class ParseError : public OrngError {
    std::string msg;
    unsigned int column;

  public:
    ParseError(std::string msg, unsigned int column)
        : msg{msg}, column{column} {}
    std::string caused_by();
};

class Parser {
    Lexer lexer;
    Token curtok;
    Token peektok;

    void next();
    Token peek();

    std::complex<long> parse_complex();
    long parse_integer();

    Ast *expr(bool paren_ok);
    Ast *operand(bool paren_ok);
    Ast *atom();
    Ast *vector();

  public:
    Parser(char *source)
        : lexer{Lexer{source}}, curtok{lexer.lex()}, peektok{lexer.lex()} {}

    Ast *parse();
};

#endif