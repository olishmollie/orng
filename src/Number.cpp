#include "Number.hpp"

Number::Number(long n) {
    type = NumInteger;
    integer = n;
}

Number::Number(long double n) {
    type = NumReal;
    real = n;
}

Number::Number(std::complex<long double> n) {
    type = NumComplex;
    complex = n;
}

std::ostream &operator<<(std::ostream &os, const Number &n) {
    switch (n.type) {
    case NumInteger:
        os << n.integer;
        break;
    case NumReal:
        os << n.real;
        break;
    case NumComplex:
        os << n.complex.real() << (n.complex.imag() > 0 ? "+" : "")
           << n.complex.imag() << "i";
    }
    return os;
}