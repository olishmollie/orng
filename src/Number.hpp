#ifndef NUMBER_H
#define NUMBER_H

#include <complex>

enum NumberType {
    NumInteger,
    NumReal,
    NumComplex,
};

struct Number {
    NumberType type;
    union {
        long integer;
        long double real;
        std::complex<long double> complex;
    };

    Number();
    Number(long integer);
    Number(long double real);
    Number(std::complex<long double> complex);

    friend std::ostream &operator<<(std::ostream &os, const Number &n);
};

#endif