#include "Parser.h"

#include <complex>
#include <iostream>
#include <sstream>
#include <vector>

ParseError::ParseError(std::string msg_, unsigned int column_)
    : msg{msg_}, column{column_} {}

std::string ParseError::caused_by() {
    std::ostringstream os;
    os << "parse error: " << msg << " (column: " << column << ")";
    return os.str();
}

Parser::Parser(char *source)
    : lexer{Lexer{source}}, curtok{lexer.lex()}, peektok{lexer.lex()} {}

void Parser::next() {
    curtok = peektok;
    peektok = lexer.lex();
}

Ast *Parser::parse() {
    return expr(false);
}

Ast *Parser::expr(bool paren_ok) {
    Ast *left = operand(paren_ok);
    Token tok = curtok;
    switch (tok.type) {
    case Eof:
    case TokRightParen:
        if (!paren_ok && tok.type == TokRightParen) {
            throw ParseError("unexpected ')'", curtok.position);
        }
        return left;
    case TokOperator:
        next();
        return new BinaryAst(tok, left, expr(paren_ok));
    default:
        throw ParseError("unexpected token", curtok.position);
    }
}

// number
// vector
Ast *Parser::operand(bool paren_ok) {
    Ast *left;
    Token tok = curtok;
    switch (tok.type) {
    case TokOperator:
        next();
        left = new UnaryAst(tok, expr(paren_ok));
        break;
    case TokInteger:
    case TokReal:
    case TokComplex:
    case TokLeftParen:
        left = atom();
        break;
    default:
        throw ParseError("unexpected token", tok.position);
    }

    return left;
}

// vector
// '(' expr ')'
Ast *Parser::atom() {
    Ast *inner;
    Token tok = curtok;
    switch (tok.type) {
    case TokInteger:
    case TokReal:
    case TokComplex:
        return vector();
    case TokLeftParen:
        next();
        inner = expr(true);
        if (curtok.type != TokRightParen) {
            throw ParseError("expected ')'", curtok.position);
        }
        next();
        break;
    default:
        throw ParseError("unknown token type", curtok.position);
    }

    return inner;
}

static bool is_number_type(Token tok) {
    return tok.type == TokInteger || tok.type == TokReal ||
           tok.type == TokComplex;
}

Number Parser::parse_integer() {
    size_t endptr;
    return std::stol(curtok.lexeme, &endptr);
}

Number Parser::parse_real() {
    return std::stold(curtok.lexeme, nullptr);
}

Number Parser::parse_complex() {
    std::istringstream is(curtok.lexeme);
    std::string buf;

    long double re;
    long double im;

    std::getline(is, buf, 'j');
    re = stold(buf, nullptr);
    std::getline(is, buf);
    im = stold(buf, nullptr);

    return std::complex<long double>(re, im);
}

// vector
// scalar
Ast *Parser::vector() {
    Token tok = curtok;
    std::vector<Number> *vec = new std::vector<Number>();
    while (is_number_type(curtok)) {
        if (curtok.type == TokInteger) {
            vec->push_back(parse_integer());
        } else if (curtok.type == TokReal) {
            vec->push_back(parse_real());
        } else if (curtok.type == TokComplex) {
            vec->push_back(parse_complex());
        }
        next();
    }

    if (vec->size() == 1) {
        Ast *result = new LiteralAst(tok, (*vec)[0]);
        delete vec;
        return result;
    }
    return new LiteralAst(tok, vec);
}