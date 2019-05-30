#include "Matrix.hpp"

#include <iostream>

Matrix::Matrix(unsigned long rows_, unsigned long cols_)
    : rows{rows_}, cols{cols_} {
    data =
        new std::vector<Number>(static_cast<size_t>(rows_ * cols_), Number());
}

Matrix::Matrix(std::vector<Number> *data_) : data{data_} {
    rows = 1;
    cols = data->size();
}

Matrix::~Matrix() {
    delete data;
}

unsigned long Matrix::size() const {
    return rows * cols;
}

Number Matrix::at(unsigned long index) const {
    return data->at(index);
}
