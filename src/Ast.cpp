#include "Ast.h"

#include <sstream>

#define INDENT "    "

LiteralAst::LiteralAst(Token root_, Value *value_)
    : root{root_}, value{value_} {}

std::string LiteralAst::to_string(int depth) {
    std::ostringstream os;
    os << "<LiteralAst " << *value << ">";
    return os.str();
}

Value *LiteralAst::eval() {
    return value;
}

LiteralAst::~LiteralAst() {
    delete value;
}

std::string UnaryAst::to_string(int depth) {
    std::ostringstream os;
    os << "<UnaryAst '" << root.lexeme << "'" << std::endl;
    for (int i = 0; i < depth + 1; i++) {
        os << INDENT;
    }
    os << next->to_string(depth + 1) << ">";
    return os.str();
}

Value *UnaryAst::eval() {
    return nullptr;
}

UnaryAst::~UnaryAst() {
    if (next) {
        delete next;
    }
}

std::string BinaryAst::to_string(int depth) {
    std::ostringstream os;
    os << "<BinaryAst '" << root.lexeme << "'" << std::endl;
    for (int i = 0; i < depth + 1; i++) {
        os << INDENT;
    }
    os << left->to_string(depth + 1) << std::endl;
    for (int i = 0; i < depth + 1; i++) {
        os << INDENT;
    }
    os << right->to_string(depth + 1);
    return os.str();
}

Value *BinaryAst::eval() {
    return nullptr;
}

BinaryAst::~BinaryAst() {
    if (left) {
        delete left;
    }
    if (right) {
        delete right;
    }
}
