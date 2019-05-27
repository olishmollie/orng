#ifndef PARSER_H
#define PARSER_H

#include "Ast.h"
#include "Lexer.h"

class Parser {
    Lexer lexer;
    Token curtok;
    Token peektok;

    void next();
    Token peek();

    Ast *expr();
    Ast *operand();
    Ast *atom();
    Ast *vector();

  public:
    Parser(char *source)
        : lexer{Lexer{source}}, curtok{lexer.lex()}, peektok{lexer.lex()} {}

    Ast *parse();
};

#endif