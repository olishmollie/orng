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
    unsigned long len;
    union {
        long integer;
        long double real;
        std::complex<long double> complex;
    };

    Number();
    Number(long integer);
    Number(long double real);
    Number(std::complex<long double> complex);

    unsigned long length();

    bool is_integer() const;
    bool is_real() const;
    bool is_complex() const;

    bool operator<(int n) const;
    bool operator<=(int n) const;

    bool operator==(const Number &n) const;
    bool operator<(const Number &n) const;
    bool operator<=(const Number &n) const;
    bool operator>(const Number &n) const;
    bool operator>=(const Number &n) const;

    Number &operator+=(Number &n);
    Number &operator-=(Number &n);
    Number &operator*=(Number &n);
    Number &operator/=(Number &n);

    Number operator+(Number &n) const;
    Number operator-(Number &n) const;
    Number operator*(Number &n) const;
    Number operator/(Number &n) const;

    Number &operator*=(int x);
    Number &operator*=(long double x);

    Number operator*(long double x);

    Number abs_val() const;

    friend std::ostream &operator<<(std::ostream &os, const Number &n);
};

#endif