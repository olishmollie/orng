#ifndef LEXER_H
#define LEXER_H

#include "Error.h"
#include "Token.h"

#include <string>
#include <vector>

class LexicalError : public OrngError {
    std::string msg;
    unsigned int column;

  public:
    LexicalError(std::string msg, unsigned int column)
        : msg{msg}, column{column} {}
    std::string caused_by();
};

class Lexer {
    int start;
    int end;
    std::string source;

    void ignore_char();
    char next_char();
    char peek_char();
    char prev_char();

    std::string lex_integer();

    Token lex_scalar();
    Token lex_operator();
    Token lex_paren();

  public:
    Lexer(char *source);
    Lexer(std::string source);
    Token lex();
    bool eof();
};

#endif