#include "Value.h"

Value::Value(long n) {
    type = Scalar;
    scalar = n;
}

Value::Value(std::vector<long> v) {
    type = Vector;
    vec = v;
}

Value::Value(std::string op) {
    type = Operation;
    operation = op;
}

Value::Value(const Value &val) {
    type = val.type;
    switch (type) {
        case Scalar:
            scalar = val.scalar;
            break;
        case Vector: {
            std::vector<long> v;
            for (int i = 0; i < val.vec.size(); i++) {
                v[i] = val.vec[i];
            }
            vec = v;
            break;
        }
        case Operation:
            operation = val.operation;
            break;
    }
}