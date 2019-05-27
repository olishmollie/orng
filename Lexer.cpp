#include "Lexer.h"

#include <ctype.h>
#include <vector>

std::string LexicalError::caused_by() {
    std::ostringstream os;
    os << "lexical error: " << msg << " (column: " << column << ")";
    return os.str();
}

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

static bool is_space(char c) {
    return c == ' ' || c == '\n' || c == '\t';
}

static bool is_paren(char c) {
    return c == '[' || c == ']' || c == '(' || c == ')';
}

static bool is_op(char c) {
    return c == '+' || c == '-' || c == '*' || c == '%' || c == '!' ||
           c == '@' || c == '/' || c == '?' || c == '<';
}

static bool is_delim(char c) {
    return c == ' ' || c == '\n' || c == '_' || c == '(' || c == ')' ||
           c == '[' || c == ']' || c == '\0' || is_op(c);
}

static bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

Token Lexer::lex() {
    char c;

    while ((c = peek_char())) {
        if (is_space(c)) {
            ignore_char();
            continue;
        } else if (is_paren(c)) {
            return lex_paren();
        } else if (is_digit(c)) {
            return lex_scalar();
        } else if (is_op(c)) {
            return lex_operator();
        }
        throw LexicalError("unknown character", end + 1);
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
    c = prev_char();

    if (!eof() && !is_delim(c)) {
        throw LexicalError("invalid number syntax", end + 1);
    }

    return Token(TokInteger, buf, position);
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