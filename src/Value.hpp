#ifndef VALUE_H
#define VALUE_H

#include "Matrix.hpp"
#include "Number.hpp"

#include <iostream>
#include <string>
#include <vector>

enum ValueType { ValIdentifier, ValMatrix };

struct Value {
    ValueType type;
    union {
        std::string identifier;
        Matrix *matrix;
    };

    Value(std::string identifier);
    Value(std::vector<Number> *vec);
    ~Value();

    bool is_scalar();

    friend std::ostream &operator<<(std::ostream &os, const Value &v);
};

#endif
