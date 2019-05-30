#ifndef VALUE_H
#define VALUE_H

#include "Number.hpp"

#include <iostream>
#include <string>
#include <vector>

enum ValueType { Identifier, Vector };

struct Value {
    ValueType type;
    union {
        std::string identifier;
        std::vector<Number> *vec;
    };

    Value(std::string identifier);
    Value(std::vector<Number> *vec);
    ~Value();

    bool is_scalar();
    Number at(unsigned long i);

    friend std::ostream &operator<<(std::ostream &os, const Value &v);
};

#endif
