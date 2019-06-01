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

bool Number::is_real() const {
    return type == NumReal;
}

bool Number::is_complex() const {
    return type == NumComplex;
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

Number &Number::operator+=(Number &n) {
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

Number &Number::operator-=(Number &n) {
    n *= -1;
    *this += n;
    return *this;
}

Number &Number::operator*=(Number &n) {
    NumberType tmp = type;

    switch (tmp) {
    case NumInteger:
        switch (n.type) {
        case NumInteger:
            integer *= n.integer;
            break;
        case NumReal:
            type = NumReal;
            real = integer * n.real;
            break;
        case NumComplex:
            type = NumComplex;
            complex = static_cast<long double>(integer) * n.complex;
        }
        break;
    case NumReal:
        switch (n.type) {
        case NumInteger:
            real *= n.integer;
            break;
        case NumReal:
            real *= n.real;
            break;
        case NumComplex:
            type = NumComplex;
            complex = real * n.complex;
        }
        break;
    case NumComplex:
        switch (n.type) {
        case NumInteger:
            complex *= static_cast<long double>(n.integer);
            break;
        case NumReal:
            complex *= n.real;
            break;
        case NumComplex:
            complex *= n.complex;
        }
    }

    return *this;
}

Number &Number::operator/=(Number &n) {
    NumberType tmp = type;

    switch (tmp) {
    case NumInteger:
        switch (n.type) {
        case NumInteger:
            type = NumReal;
            real = static_cast<long double>(integer) / n.integer;
            break;
        case NumReal:
            type = NumReal;
            real = integer / n.real;
            break;
        case NumComplex:
            type = NumComplex;
            complex = static_cast<long double>(integer) / n.complex;
        }
        break;
    case NumReal:
        switch (n.type) {
        case NumInteger:
            real /= n.integer;
            break;
        case NumReal:
            real /= n.real;
            break;
        case NumComplex:
            type = NumComplex;
            complex = real / n.complex;
        }
        break;
    case NumComplex:
        switch (n.type) {
        case NumInteger:
            complex /= static_cast<long double>(n.integer);
            break;
        case NumReal:
            complex /= n.real;
            break;
        case NumComplex:
            complex /= n.complex;
        }
    }

    return *this;
}

Number Number::operator+(Number &n) const {
    Number sum = *this;
    sum += n;
    return sum;
}

Number Number::operator-(Number &n) const {
    Number sub = *this;
    sub -= n;
    return sub;
}

Number Number::operator*(Number &n) const {
    Number mul = *this;
    mul *= n;
    return mul;
}

Number Number::operator/(Number &n) const {
    Number div = *this;
    div /= n;
    return div;
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

Number Number::operator*(long double x) {
    Number n = *this;
    n *= x;
    return n;
}

Number Number::abs_val() const {
    switch (type) {
    case NumInteger:
        return Number(abs(real));
    case NumReal:
        return Number(abs(real));
    case NumComplex:
        return Number(abs(complex));
    }
}

static void print_int(std::ostream &os, long n) {
    if (n < 0) {
        os << "_";
    }
    os << abs(n);
}

static void print_real(std::ostream &os, long double real) {
    if (real < 0) {
        os << "_";
    }
    os << abs(real);
}

static void print_complex(std::ostream &os, std::complex<long double> complex) {
    print_real(os, complex.real());
    os << "j";
    print_real(os, complex.imag());
}

std::ostream &operator<<(std::ostream &os, const Number &n) {
    std::ios_base::fmtflags f(os.flags());

    switch (n.type) {
    case NumInteger:
        print_int(os, n.integer);
        break;
    case NumReal:
        // os << std::setprecision(9) << std::fixed;
        print_real(os, n.real);
        break;
    case NumComplex:
        print_complex(os, n.complex);
    }

    os.flags(f);

    return os;
}