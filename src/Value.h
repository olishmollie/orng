#ifndef VALUE_H
#define VALUE_H

#include "Number.h"

#include <iostream>
#include <string>
#include <vector>

enum ValueType { Identifier, Scalar, Vector };

struct Value {
    ValueType type;
    union {
        std::string identifier;
        Number scalar;
        std::vector<Number> *vec;
    };

    Value(std::string identifier);
    Value(Number scalar);
    Value(std::vector<Number> *vec);

    Value(const Value &v);
    ~Value();

    friend std::ostream &operator<<(std::ostream &os, const Value &v);
};

#endif
