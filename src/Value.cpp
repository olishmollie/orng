#include "Value.hpp"

Value::Value(std::string identifier_)
    : type{ValIdentifier}, identifier{identifier_} {}

Value::Value(std::vector<Number> *vec) : type{ValMatrix} {
    matrix = new Matrix(vec);
}

Value::~Value() {
    if (type == ValMatrix) {
        delete matrix;
    }
}

bool Value::is_scalar() {
    return type == ValMatrix && matrix->size() == 1;
}

std::ostream &operator<<(std::ostream &os, const Value &v) {
    switch (v.type) {
    case ValIdentifier:
        os << v.identifier;
        break;
    case ValMatrix:
        for (unsigned long i = 0; i < v.matrix->size(); i++) {
            os << v.matrix->at(i);
            if (i < v.matrix->size() - 1) {
                os << " ";
            }
        }
    }
    return os;
}