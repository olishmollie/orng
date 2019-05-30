#ifndef MATRIX_H
#define MATRIX_H

#include "Number.hpp"

#include <vector>

#define NIL std::unique_ptr<Matrix>(new Matrix())

typedef std::vector<unsigned long> Shape;

class Matrix {
  private:
    Shape *shape;

    std::vector<Number> *data;

  public:
    Matrix(); // nil value
    Matrix(unsigned long rows, unsigned long cols);
    Matrix(std::vector<Number> *data);
    ~Matrix();

    bool is_scalar();
    bool is_integer();
    bool is_real();
    bool is_complex();

    Number &at(unsigned long index) const;
    Shape *get_shape();
    unsigned long count() const;

    friend std::ostream &operator<<(std::ostream &os, const Matrix &matrix);
};

#endif