#include "Parser.h"

#include <sstream>
#include <vector>

std::string ParseError::caused_by() {
    std::ostringstream os;
    os << "parse error: " << msg << " (column: " << column << ")";
    return os.str();
}

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
    return tok.type == TokInteger || tok.type == TokComplex;
}

std::complex<long> Parser::parse_complex() {}

long Parser::parse_integer() {
    size_t endptr;
    return std::stol(curtok.lexeme, &endptr);
}

// vector
// scalar
Ast *Parser::vector() {
    Token tok = curtok;
    std::vector<long> *vec = new std::vector<long>();
    while (is_number_type(curtok)) {
        if (curtok.type == TokInteger) {
            vec->push_back(parse_integer());
        } else if (curtok.type == TokComplex) {
            // vec->push_back(parse_complex());
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