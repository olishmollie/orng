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

Value Ast::eval() {
    return root->eval();
}

Ast::~Ast() {
    delete root;
}

LiteralExpr::LiteralExpr(Token root_, Matrix *matrix_)
    : root{root_}, matrix{matrix_} {}

std::string LiteralExpr::to_string(int depth) {
    std::ostringstream os;
    os << "<LiteralExpr " << *matrix << ">";
    return os.str();
}

Value LiteralExpr::eval() {
    return std::unique_ptr<Matrix>(matrix);
}

LiteralExpr::~LiteralExpr() {}

UnaryExpr::UnaryExpr(Token root_, Expr *next_) : root{root_}, next{next_} {}

std::string UnaryExpr::to_string(int depth) {
    std::ostringstream os;
    os << "<UnaryExpr '" << root.lexeme << "'" << std::endl;
    for (int i = 0; i < depth + 1; i++) {
        os << INDENT;
    }
    os << next->to_string(depth + 1) << ">";
    return os.str();
}

Value UnaryExpr::eval() {
    if (root.lexeme == "!") {
        return iota();
    } else if (root.lexeme == "#") {
        return shape();
    }
    return NIL;
}

Value UnaryExpr::iota() {
    Value arg = next->eval();
    if (!arg->is_integer()) {
        throw "domain error";
    }

    long size = arg->at(0).integer;
    if (size < 0) {
        throw "domain error";
    }
    if (size == 0) {
        return NIL;
    }

    std::vector<Number> *vec = new std::vector<Number>();
    for (long i = 0; i < size; i++) {
        vec->push_back(Number(i + 1));
    }

    return std::unique_ptr<Matrix>(new Matrix(vec));
}

Value UnaryExpr::shape() {
    Value arg = next->eval();
}

UnaryExpr::~UnaryExpr() {
    if (next) {
        delete next;
    }
}

BinaryExpr::BinaryExpr(Token root_, Expr *left_, Expr *right_)
    : root{root_}, left{left_}, right{right_} {}

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

Value BinaryExpr::eval() {
    if (root.lexeme == "#") {
        return reshape();
    }
    return NIL;
}

Value BinaryExpr::reshape() {
    Value larg = left->eval();

    Shape *shape = larg->get_shape();

    return NIL;
}

BinaryExpr::~BinaryExpr() {
    if (left) {
        delete left;
    }
    if (right) {
        delete right;
    }
}
