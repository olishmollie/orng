#include "Parser.hpp"

#include <complex>
#include <iostream>
#include <sstream>
#include <vector>

ParseError::ParseError(std::string msg_, std::string source_,
                       unsigned int column_)
    : msg{msg_}, source{source_}, column{column_} {}

std::string ParseError::caused_by() {
    std::ostringstream os;
    os << "parse error: " << msg << std::endl;
    os << source << std::endl;
    for (int i = 1; i < column; i++) {
        os << " ";
    }
    os << "^";
    return os.str();
}

Parser::Parser(char *source_)
    : source{static_cast<std::string>(source_)}, lexer{Lexer{source_}},
      curtok{lexer.lex()}, peektok{lexer.lex()} {}

void Parser::next() {
    curtok = peektok;
    peektok = lexer.lex();
}

Ast Parser::parse() {
    return Ast{expr(false)};
}

Expr *Parser::expr(bool paren_ok) {
    Expr *left = operand(paren_ok);
    Token tok = curtok;
    switch (tok.type) {
    case Eof:
    case TokRightParen:
        if (!paren_ok && tok.type == TokRightParen) {
            // TODO: RAII solution to this?
            delete left;
            throw ParseError("unexpected token", source, curtok.position);
        }
        return left;
    case TokOperator:
        next();
        return new BinaryExpr(tok, left, expr(paren_ok));
    default:
        throw ParseError("unexpected token", source, curtok.position);
    }
}

// operand:
//   atom
//   unop operand
Expr *Parser::operand(bool paren_ok) {
    Expr *left;
    Token tok = curtok;
    switch (tok.type) {
    case TokOperator:
        next();
        left = new UnaryExpr(tok, expr(paren_ok));
        break;
    case TokIdentifier:
    case TokInteger:
    case TokReal:
    case TokComplex:
    case TokLeftParen:
        left = atom();
        break;
    case Eof:
        return new LiteralExpr(curtok, new Matrix());
    default:
        std::cout << "token = " << tok << std::endl;
        throw ParseError("unexpected token", source, tok.position);
    }

    return left;
}

// atom:
//   scalar
//   vector
//   identifier
//   '(' expr ')'
Expr *Parser::atom() {
    Expr *inner;
    Token tok = curtok;
    switch (tok.type) {
    case TokIdentifier:
        // next();
        throw ParseError("unimplmented feature", source, tok.position);
    case TokInteger:
    case TokReal:
    case TokComplex:
        return vector();
    case TokLeftParen:
        next();
        inner = expr(true);
        if (curtok.type != TokRightParen) {
            // TODO: RAII solution to this?
            delete inner;
            throw ParseError("expected ')'", source, curtok.position);
        }
        next();
        break;
    default:
        throw ParseError("unknown token type", source, curtok.position);
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
Expr *Parser::vector() {
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

    return new LiteralExpr(tok, new Matrix(vec));
}