#include "Lexer.hpp"

#include <sstream>
#include <vector>

LexicalError::LexicalError(std::string msg_, std::string source_,
                           unsigned int column_)
    : msg{msg_}, source{source_}, column{column_} {}

std::string LexicalError::caused_by() {
    std::ostringstream os;
    os << "lexical error: " << msg << std::endl;
    os << source << std::endl;
    for (int i = 0; i < column; i++) {
        os << " ";
    }
    os << "^";
    return os.str();
}

Lexer::Lexer(char *source_) : source{(std::string)source_}, start{0}, end{0} {}

Lexer::Lexer(std::string source) {
    this->source = source;
    start = end = 0;
}

void Lexer::ignore_char() {
    start = ++end;
}

char Lexer::next_char() {
    if (!eof()) {
        return source[end++];
    }
    return 0;
}

char Lexer::peek_char() {
    if (!eof()) {
        return source[end];
    }
    return 0;
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

static bool is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

Token Lexer::lex() {
    char c;

    while ((c = peek_char())) {
        if (is_space(c)) {
            ignore_char();
            continue;
        } else if (c == '_' || is_digit(c)) {
            return lex_scalar();
        } else if (is_alpha(c)) {
            return lex_identifier();
        } else if (is_paren(c)) {
            return lex_paren();
        } else if (is_op(c)) {
            return lex_operator();
        }
        throw LexicalError("unknown character", source, end + 1);
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
        throw LexicalError("malformed number", source, end);
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

    // prepare for next call to lex()
    prev_char();

    if (!eof() && !is_delim(c)) {
        throw LexicalError("malformed number", source, end);
    }

    if (complex) {
        return Token(TokComplex, buf, position);
    } else if (real) {
        return Token(TokReal, buf, position);
    }

    return Token(TokInteger, buf, position);
}

Token Lexer::lex_identifier() {
    int position = end + 1;
    std::string buf;

    char c = next_char();
    while (!eof() && isalnum(c)) {
        buf.append(1, c);
        c = next_char();
    }
    prev_char();

    return Token(TokIdentifier, buf, position);
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