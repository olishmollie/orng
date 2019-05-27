#ifndef NUMBER_H
#define NUMBER_H

#include <complex>

enum NumberType {
    NumInteger,
    NumComplex,
};

struct Number {
    NumberType type;
    union {
        long integer;
        std::complex<long double> complex;
    };

    Number(long integer);
    Number(std::complex<long double> complex);

    friend std::ostream &operator<<(std::ostream &os, const Number &n);
};

#endif