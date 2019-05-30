#ifndef NUMBER_H
#define NUMBER_H

#include <complex>
#include <iostream>

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

    bool is_integer() const;

    bool operator<(int n) const;
    bool operator<=(int n) const;

    bool operator==(const Number &n) const;
    bool operator<(const Number &n) const;
    bool operator<=(const Number &n) const;
    bool operator>(const Number &n) const;
    bool operator>=(const Number &n) const;

    Number operator+(const Number &n) const;

    Number &operator+=(const Number &n);

    Number &operator*=(int x);
    Number &operator*=(long double x);

    friend std::ostream &operator<<(std::ostream &os, const Number &n);
};

#endif