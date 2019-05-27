#include "Parser.h"

#include <vector>

void Parser::next() {
    curtok = peektok;
    peektok = lexer.lex();
}

Ast *Parser::parse() {
    return expr();
}

Ast *Parser::expr() {
    Ast *left = operand();
    Token tok = curtok;
    switch (tok.type) {
    case Eof:
    case TokRightParen:
        return left;
    case TokOperator:
        next();
        return new BinaryAst(tok, left, expr());
    default:
        throw "expr(): unexpected token";
    }
}

// number
// vector
Ast *Parser::operand() {
    Ast *left;
    Token tok = curtok;
    switch (tok.type) {
    case TokOperator:
        next();
        left = new UnaryAst(tok, expr());
        break;
    case TokScalar:
    case TokLeftParen:
        left = atom();
        break;
    default:
        throw "operand(): unknown token type";
    }

    return left;
}

// vector
// '(' expr ')'
Ast *Parser::atom() {
    Ast *inner;
    Token tok = curtok;
    switch (tok.type) {
    case TokScalar:
        return vector();
    case TokLeftParen:
        next();
        inner = expr();
        if (curtok.type != TokRightParen) {
            throw "expected ')'";
        }
        next();
        break;
    default:
        throw "atom(): unknown token type";
    }

    return inner;
}

// vector
// scalar
Ast *Parser::vector() {
    Token tok = curtok;
    std::vector<long> *vec = new std::vector<long>();
    while (curtok.type == TokScalar) {
        size_t endptr;
        long val = std::stol(curtok.lexeme, &endptr);
        // TODO: figure this out
        // if (endptr) {
        //     throw "invalid number syntax";
        // }
        vec->push_back(val);
        next();
    }

    if (vec->size() == 1) {
        Ast *result = new LiteralAst(tok, (*vec)[0]);
        delete vec;
        return result;
    }
    return new LiteralAst(tok, vec);
}