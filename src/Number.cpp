#include "Number.hpp"

#include <iomanip>

Number::Number() {
    type = NumInteger;
    integer = 0;
}

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

bool Number::is_integer() const {
    return type == NumInteger;
}

bool Number::operator<(int n) const {
    switch (type) {
    case NumInteger:
        return integer < n;
    case NumReal:
        return real < n;
    default:
        throw "domain error";
    }
}

bool Number::operator<=(int n) const {
    switch (type) {
    case NumInteger:
        return integer <= n;
    case NumReal:
        return real <= n;
    default:
        throw "domain error";
    }
}

bool Number::operator==(const Number &n) const {
    if (type != n.type) {
        return false;
    }

    switch (type) {
    case NumInteger:
        return integer == n.integer;
    case NumReal:
        // Hmm...this could be a bad idea
        return real == n.real;
    case NumComplex:
        return complex == n.complex;
    }
}

bool Number::operator<(const Number &n) const {

    if (n.type == NumInteger) {
        switch (type) {
        case NumInteger:
            return integer < n.integer;
        case NumReal:
            return real < n.integer;
        default:
            throw "domain error";
        }
    } else if (n.type == NumReal) {
        switch (type) {
        case NumInteger:
            return integer < n.real;
        case NumReal:
            return integer < n.integer;
        default:
            throw "domain error";
        }
    }

    throw "domain error";
}

bool Number::operator<=(const Number &n) const {
    return *this < n || *this == n;
}

bool Number::operator>(const Number &n) const {

    if (n.type == NumInteger) {
        switch (type) {
        case NumInteger:
            return integer > n.integer;
        case NumReal:
            return real > n.integer;
        default:
            throw "domain error";
        }
    } else if (n.type == NumReal) {
        switch (type) {
        case NumInteger:
            return integer > n.real;
        case NumReal:
            return integer > n.integer;
        default:
            throw "domain error";
        }
    }

    throw "domain error";
}

bool Number::operator>=(const Number &n) const {
    return *this > n || *this == n;
}

Number Number::operator+(const Number &n) const {
    Number sum = *this;
    sum += n;
    return sum;
}

Number &Number::operator+=(const Number &n) {
    NumberType tmp = type;

    switch (tmp) {
    case NumInteger:
        switch (n.type) {
        case NumInteger:
            integer += n.integer;
            break;
        case NumReal:
            type = NumReal;
            real = integer + n.real;
            break;
        case NumComplex:
            type = NumComplex;
            complex = static_cast<long double>(integer) + n.complex;
        }
        break;
    case NumReal:
        switch (n.type) {
        case NumInteger:
            real += n.integer;
            break;
        case NumReal:
            real += n.real;
            break;
        case NumComplex:
            type = NumComplex;
            complex = real + n.complex;
        }
        break;
    case NumComplex:
        switch (n.type) {
        case NumInteger:
            complex += static_cast<long double>(n.integer);
            break;
        case NumReal:
            complex += n.real;
            break;
        case NumComplex:
            complex += n.complex;
        }
    }

    return *this;
}

Number &Number::operator*=(int x) {
    switch (type) {
    case NumInteger:
        integer *= x;
        break;
    case NumReal:
        real *= x;
        break;
    case NumComplex:
        complex *= x;
    }

    return *this;
}

Number &Number::operator*=(long double x) {
    NumberType tmp = type;
    type = NumReal;

    switch (tmp) {
    case NumInteger:
        real = integer * x;
        break;
    case NumReal:
        real *= x;
        break;
    default:
        throw "domain error";
    }

    return *this;
}

std::ostream &operator<<(std::ostream &os, const Number &n) {
    std::ios_base::fmtflags f(os.flags());

    switch (n.type) {
    case NumInteger:
        os << n.integer;
        break;
    case NumReal:
        // os << std::setprecision(9) << std::fixed;
        os << n.real;
        break;
    case NumComplex:
        os << n.complex.real() << (n.complex.imag() > 0 ? "+" : "")
           << n.complex.imag() << "i";
    }

    os.flags(f);

    return os;
}