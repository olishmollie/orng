#include "Value.hpp"

Value::Value(std::string identifier_)
    : type{Identifier}, identifier{identifier_} {}

Value::Value(std::vector<Number> *vec_) : type{Vector}, vec{vec_} {}

Value::~Value() {
    if (type == Vector) {
        delete vec;
    }
}

bool Value::is_scalar() {
    return type == Vector && vec->size() == 1;
}

Number Value::at(unsigned long i) {
    return vec->at(i);
}

std::ostream &operator<<(std::ostream &os, const Value &v) {
    switch (v.type) {
    case Identifier:
        os << v.identifier;
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