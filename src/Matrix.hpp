#ifndef MATRIX_H
#define MATRIX_H

#include "Number.hpp"

#include <vector>

#define NIL std::unique_ptr<Matrix>(new Matrix())

typedef std::vector<unsigned int> Shape;

class Matrix {
  private:
    Shape *shape;

    std::vector<Number> *data;

  public:
    Matrix(); // nil value
    Matrix(const Matrix &m);

    Matrix(Shape *shape, std::vector<Number> *data);
    Matrix(std::vector<Number> *data);
    ~Matrix();

    bool is_vector() const;
    bool is_scalar() const;
    bool is_integer() const;
    bool is_real() const;
    bool is_complex() const;
    bool is_nil() const;

    Number &at(unsigned long index) const;
    Shape *get_shape();
    unsigned long count() const;

    // Populates an empty shape and returns size of a matrix w/ that shape
    unsigned long to_shape(Shape *s) const;

    friend std::ostream &operator<<(std::ostream &os, const Matrix &matrix);
};

#endif