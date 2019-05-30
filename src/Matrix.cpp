#include "Matrix.hpp"

#include <iostream>

Matrix::Matrix() {
    shape = nullptr;
    data = nullptr;
}

Matrix::Matrix(unsigned long rows_, unsigned long cols_) {
    shape = new Shape{rows_, cols_};
    data =
        new std::vector<Number>(static_cast<size_t>(rows_ * cols_), Number());
}

Matrix::Matrix(std::vector<Number> *data_) : data{data_} {
    shape = new Shape{1, data->size()};
}

Matrix::~Matrix() {
    delete shape;
    delete data;
}

bool Matrix::is_scalar() {
    return shape->size() == 2 && shape->at(0) == 1 && shape->at(1) == 1;
}

bool Matrix::is_integer() {
    return is_scalar() && (*data)[0].type == NumInteger;
}

bool Matrix::is_real() {
    return is_scalar() && (*data)[0].type == NumReal;
}

bool Matrix::is_complex() {
    return is_scalar() && (*data)[0].type == NumReal;
}

Number &Matrix::at(unsigned long index) const {
    return data->at(index);
}

Shape *Matrix::get_shape() {
    return shape;
}

unsigned long Matrix::count() const {
    unsigned long count = 1;
    for (int i = 0; i < shape->size(); i++) {
        count *= shape->at(i);
    }
    return count;
}

std::ostream &operator<<(std::ostream &os, const Matrix &matrix) {
    // don't print nil value
    if (matrix.shape == nullptr) {
        return os;
    }
    for (unsigned long i = 0; i < matrix.count(); i++) {
        os << matrix.at(i);
        if (i < matrix.count() - 1) {
            os << " ";
        }
    }
    os << std::endl;
    return os;
}