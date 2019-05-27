#include "Ast.h"

#include <sstream>

#define INDENT "    "

LiteralAst::LiteralAst(Token root, long n) : root{root}, scalar{n} {
    is_vector = false;
}

LiteralAst::LiteralAst(Token root, std::vector<long> *vec) : root{root}, vec{vec} {
    is_vector = true;
}

std::string LiteralAst::to_string(int depth) {
    std::ostringstream os;

    os << "<LiteralAst ";

    if (is_vector) {
        os << "[";
        for (int i = 0; i < vec->size(); i++) {
            os << (*vec)[i];
            if (i != vec->size() - 1) {
                os << " ";
            }
        }
        os << "]";
    } else {
        os << scalar;
    }

    os << ">";

    return os.str();
}

LiteralAst::~LiteralAst() {
    if (is_vector) {
        delete vec;
    }
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

BinaryAst::~BinaryAst() {
    if (left) {
        delete left;
    }
    if (right) {
        delete right;
    }
}
