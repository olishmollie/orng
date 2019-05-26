#ifndef LEXER_H
#define LEXER_H

#include "Token.h"

#include <any>
#include <string>
#include <vector>

class Lexer {
  private:
    int start;
    int end;
    std::string source;

    void ignore_char();
    char next_char();
    char peek_char();
    char prev_char();

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