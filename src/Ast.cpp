#include "Ast.hpp"

#include <sstream>

#define INDENT "    "

RuntimeError::RuntimeError(std::string msg_, std::string source_,
                           unsigned int column_)
    : msg{msg_}, source{source_}, column{column_} {}

std::string RuntimeError::caused_by() {
    std::ostringstream os;
    os << "parse error: " << msg << std::endl;
    os << source << std::endl;
    for (int i = 1; i < column; i++) {
        os << " ";
    }
    os << "^";
    return os.str();
}

std::string Ast::to_string(int depth) {
    return root->to_string();
}

std::unique_ptr<Value> Ast::eval() {
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

std::unique_ptr<Value> LiteralExpr::eval() {
    return std::unique_ptr<Value>(value);
}

LiteralExpr::~LiteralExpr() {}

std::string UnaryExpr::to_string(int depth) {
    std::ostringstream os;
    os << "<UnaryExpr '" << root.lexeme << "'" << std::endl;
    for (int i = 0; i < depth + 1; i++) {
        os << INDENT;
    }
    os << next->to_string(depth + 1) << ">";
    return os.str();
}

std::unique_ptr<Value> UnaryExpr::eval() {
    if (root.lexeme == "!") {
        return iota();
    }
    return nullptr;
}

std::unique_ptr<Value> UnaryExpr::iota() {
    std::unique_ptr<Value> arg = next->eval();
    if (!arg->is_scalar() && arg->matrix->at(0).type != NumInteger) {
        throw "domain error";
    }

    long size = arg->matrix->at(0).integer;
    if (size < 0) {
        throw "domain error";
    }

    std::vector<Number> *vec = new std::vector<Number>();
    for (long i = 0; i < size; i++) {
        vec->push_back(Number(i + 1));
    }

    return std::unique_ptr<Value>(new Value(vec));
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

std::unique_ptr<Value> BinaryExpr::eval() {
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
