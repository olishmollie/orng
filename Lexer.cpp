#include "Lexer.h"

#include <ctype.h>
#include <vector>

Lexer::Lexer(char *source) {
    this->source = static_cast<std::string>(source);
    start = end = 0;
}

Lexer::Lexer(std::string source) {
    this->source = source;
    start = end = 0;
}

void Lexer::ignore_char() {
    start = ++end;
}

char Lexer::next_char() {
    char next{source[end]};
    if (!eof()) {
        end++;
    }
    return next;
}

char Lexer::peek_char() {
    return source[end];
}

char Lexer::prev_char() {
    return source[--end];
}

Token Lexer::lex() {
    char c;
    while ((c = peek_char())) {
        switch (c) {
        case ' ':
        case '\n':
        case '\t':
            ignore_char();
            break;
        case '[':
        case ']':
        case '(':
        case ')':
            return lex_paren();
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return lex_scalar();
        case '+':
        case '-':
        case '*':
        case '%':
        case '!':
        case '@':
        case '/':
        case '?':
        case '<':
            return lex_operator();
        default:
            fprintf(stderr, "lexical error: unknown character '%c'", c);
            exit(1);
        }
    }

    next_char();

    return Token(Eof, "", end);
}

bool Lexer::eof() {
    return end > source.length();
}

Token Lexer::lex_scalar() {
    int position = end + 1;
    std::string buf;

    char c = next_char();
    while (!eof() && isdigit(c)) {
        buf.append(1, c);
        c = next_char();
    }
    prev_char();

    return Token(TokScalar, buf, position);
}

Token Lexer::lex_operator() {
    int position = end + 1;
    std::string lexeme;

    char c = next_char();
    switch (c) {
    case '+':
        lexeme = "+";
        break;
    case '-':
        lexeme = "-";
        break;
    case '*':
        lexeme = "*";
        break;
    case '%':
        lexeme = "%";
        break;
    case '!':
        lexeme = "!";
        break;
    case '@':
        lexeme = "@";
        break;
    case '/':
        lexeme = "/";
        break;
    case '?':
        if (peek_char() == '.') {
            next_char();
            lexeme = "?.";
        } else {
            lexeme = "?";
        }
        break;
    case '<':
        if (peek_char() == '-') {
            next_char();
            lexeme = "<-";
        } else {
            lexeme = "<";
        }
        break;
    }

    return Token(TokOperator, lexeme, position);
}

Token Lexer::lex_paren() {
    int position = end + 1;
    TokenType type;
    std::string lexeme;

    char c = next_char();
    switch (c) {
    case '[':
        type = TokLeftBracket;
        lexeme = "[";
        break;
    case ']':
        type = TokRightBracket;
        lexeme = "]";
        break;
    case '(':
        type = TokLeftParen;
        lexeme = "(";
        break;
    case ')':
        type = TokRightParen;
        lexeme = ")";
    }

    return Token(type, lexeme, position);
}