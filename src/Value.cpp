#include "Value.h"

Value::Value(std::string identifier_)
    : type{Identifier}, identifier{identifier_} {}

Value::Value(Number scalar_) : type{Scalar}, scalar{scalar_} {}

Value::Value(std::vector<Number> *vec_) : type{Vector}, vec{vec_} {}

Value::~Value() {
    if (type == Vector) {
        delete vec;
    }
}

std::ostream &operator<<(std::ostream &os, const Value &v) {
    switch (v.type) {
    case Identifier:
        os << v.identifier;
        break;
    case Scalar:
        os << v.scalar;
        break;
    case Vector:
        for (int i = 0; i < v.vec->size(); i++) {
            os << (*v.vec)[i];
            if (i < v.vec->size() - 1) {
                os << " ";
            }
        }
    }
    return os;
}