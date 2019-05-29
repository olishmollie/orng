#include "Ast.h"

#include <sstream>

#define INDENT "    "

std::string Ast::to_string(int depth) {
    return root->to_string();
}

Value *Ast::eval() {
    return root->eval();
}

Ast::~Ast() {
    delete root;
}

LiteralExpr::LiteralExpr(Token root_, Value *value_)
    : root{root_}, value{value_} {}

std::string LiteralExpr::to_string(int depth) {
    std::ostringstream os;
    os << "<LiteralExpr " << *value << ">";
    return os.str();
}

Value *LiteralExpr::eval() {
    return value;
}

LiteralExpr::~LiteralExpr() {
    delete value;
}

std::string UnaryExpr::to_string(int depth) {
    std::ostringstream os;
    os << "<UnaryExpr '" << root.lexeme << "'" << std::endl;
    for (int i = 0; i < depth + 1; i++) {
        os << INDENT;
    }
    os << next->to_string(depth + 1) << ">";
    return os.str();
}

Value *UnaryExpr::eval() {
    return nullptr;
}

UnaryExpr::~UnaryExpr() {
    if (next) {
        delete next;
    }
}

std::string BinaryExpr::to_string(int depth) {
    std::ostringstream os;
    os << "<BinaryExpr '" << root.lexeme << "'" << std::endl;
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

Value *BinaryExpr::eval() {
    return nullptr;
}

BinaryExpr::~BinaryExpr() {
    if (left) {
        delete left;
    }
    if (right) {
        delete right;
    }
}
