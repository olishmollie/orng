#include "Ast.hpp"

#include <random>
#include <sstream>

#define INDENT "    "

const long double PI = 3.141592654;

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

std::unique_ptr<Matrix> Ast::eval() {
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

std::unique_ptr<Matrix> LiteralExpr::eval() {
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

std::unique_ptr<Matrix> UnaryExpr::eval() {
    if (root.lexeme == "!") {
        return iota();
    } else if (root.lexeme == "#") {
        return shape();
    } else if (root.lexeme == "@") {
        return pi();
    } else if (root.lexeme == "|") {
        return abs();
    } else if (root.lexeme == "?.") {
        return roll();
    }
    return NIL;
}

std::unique_ptr<Matrix> UnaryExpr::iota() {
    std::unique_ptr<Matrix> arg = next->eval();
    if (arg->is_nil() || !arg->is_integer()) {
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

std::unique_ptr<Matrix> UnaryExpr::shape() {
    std::unique_ptr<Matrix> arg = next->eval();
    if (arg->is_nil()) {
        throw "domain error";
    }

    if (arg->is_scalar()) {
        return NIL;
    }

    Shape *shape = arg->get_shape();

    std::vector<Number> *vec = new std::vector<Number>();
    for (unsigned long i = 0; i < shape->size(); i++) {
        vec->at(i) = Number((long)shape->at(i));
    }

    return std::unique_ptr<Matrix>(new Matrix(vec));
}

std::unique_ptr<Matrix> UnaryExpr::pi() {
    std::unique_ptr<Matrix> arg = next->eval();
    if (arg->is_nil()) {
        throw "domain error";
    }

    for (unsigned long i = 0; i < arg->count(); i++) {
        arg->at(i) *= PI;
    }

    return arg;
}

std::unique_ptr<Matrix> UnaryExpr::abs() {
    std::unique_ptr<Matrix> arg = next->eval();
    if (arg->is_nil()) {
        throw "domain error";
    }

    for (unsigned long i = 0; i < arg->count(); i++) {
        Number n = arg->at(i);
        if (n < 0) {
            n *= -1;
            arg->at(i) = n;
        }
    }

    return arg;
}

std::unique_ptr<Matrix> UnaryExpr::roll() {
    std::unique_ptr<Matrix> arg = next->eval();
    if (arg->is_nil()) {
        throw "domain error";
    }

    std::vector<Number> *vec = new std::vector<Number>();

    std::mt19937 mt(1729); // seed random number generator
    for (unsigned long i = 0; i < arg->count(); i++) {
        Number n = arg->at(i);
        if (!n.is_integer() || n <= 0) {
            delete vec;
            throw "domain error";
        }
        std::uniform_int_distribution<long> dist(1, n.integer);
        vec->push_back(Number(dist(mt)));
    }

    return std::unique_ptr<Matrix>(new Matrix(vec));
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

std::unique_ptr<Matrix> BinaryExpr::eval() {
    if (root.lexeme == "#") {
        return reshape();
    }
    return NIL;
}

std::unique_ptr<Matrix> BinaryExpr::reshape() {
    std::unique_ptr<Matrix> larg = left->eval();

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
