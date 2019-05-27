#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <vector>

enum ValueType { Scalar, Vector, Operation };

struct Value {
    ValueType type;
    union {
        long scalar;
        std::vector<long> vec;
        std::string operation;
    };

    Value(long scalar);
    Value(std::vector<long> vec);
    Value(std::string op);

    Value(const Value &val);

    ~Value(){};
};

#endif