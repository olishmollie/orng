#ifndef MATRIX_H
#define MATRIX_H

#include "Number.hpp"

#include <vector>

class Matrix {
  private:
    unsigned long rows;
    unsigned long cols;

    std::vector<Number> *data;

  public:
    Matrix(unsigned long rows, unsigned long cols);
    Matrix(std::vector<Number> *data);
    ~Matrix();

    unsigned long size() const;
    Number at(unsigned long index) const;
};

#endif