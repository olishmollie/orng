#include "Lexer.h"

#include <ctype.h>
#include <sstream>
#include <vector>

LexicalError::LexicalError(std::string msg_, unsigned int column_)
    : msg{msg_}, column{column_} {}

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
    return !!strchr(".:\"=-@?*%|#$<>!^&+~/\\,", c);
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
        } else if (c == '_' || is_digit(c)) {
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

std::string Lexer::lex_number(bool &real) {
    std::string buf;

    char c = next_char();
    if (c == '_') {
        buf.append(1, '-'); // for stol later
        c = next_char();
    }

    if (!is_digit(c)) {
        throw LexicalError("invalid number syntax", end);
    }

    while (!eof() && isdigit(c)) {
        buf.append(1, c);
        c = next_char();
    }

    if (!eof() && c == '.') {
        real = true;
        buf.append(1, c);
        c = next_char();
        while (!eof() && isdigit(c)) {
            buf.append(1, c);
            c = next_char();
        }
    }

    prev_char();

    return buf;
}

Token Lexer::lex_scalar() {
    int position = end + 1;

    // HACK: check for floor operator
    if (peek_char() == '_') {
        next_char();
        if (peek_char() == '.') {
            next_char();
            return Token(TokOperator, "_.", position);
        }
        prev_char();
    }

    bool complex = false;
    bool real = false;

    std::string buf = lex_number(real);

    char c = next_char();

    // complex number
    if (!eof() && c == 'j') {
        complex = true;
        buf.append(1, c);
        buf.append(lex_number(real));
        c = next_char();
    }

    if (!eof() && !is_delim(c)) {
        throw LexicalError("invalid number syntax", end);
    }

    // prepare for next call to lex()
    prev_char();

    if (complex) {
        return Token(TokComplex, buf, position);
    } else if (real) {
        return Token(TokReal, buf, position);
    }

    return Token(TokInteger, buf, position);
}

void Lexer::check_digraph(std::string &lexeme, char secondary) {
    if (peek_char() == secondary) {
        next_char();
        lexeme.append(1, secondary);
    }
}

Token Lexer::lex_operator() {
    int position = end + 1;
    std::string lexeme;

    char c = next_char();

    lexeme.append(1, c);

    switch (c) {
    case '<':
        check_digraph(lexeme, '-');
        check_digraph(lexeme, '=');
        check_digraph(lexeme, '.');
        break;
    case '>':
        check_digraph(lexeme, '=');
        check_digraph(lexeme, '.');
        break;
    case '-':
        check_digraph(lexeme, '>');
        check_digraph(lexeme, '.');
        break;
    case '~':
        check_digraph(lexeme, '^');
        check_digraph(lexeme, '&');
        check_digraph(lexeme, '=');
        check_digraph(lexeme, '.');
        break;
    case '@':
    case '?':
    case '*':
    case '%':
    case '|':
    case '#':
    case '$':
    case '!':
    case '^':
    case '&':
    case '+':
    case '/':
    case '\\':
    case ',':
        check_digraph(lexeme, '.');
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
        if (peek_char() == ']') {
            // quad operator
            next_char();
            return Token(TokOperator, "[]", position);
        } else if (peek_char() == ')') {
            // bare_quad operator
            next_char();
            return Token(TokOperator, "[)", position);
        }
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