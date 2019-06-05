#include "Matrix.hpp"

#include <iomanip>
#include <iostream>

Matrix::Matrix() {
    shape = nullptr;
    data = nullptr;
}

Matrix::Matrix(const Matrix &m) {
    shape = m.shape;
    data = m.data;
}

Matrix::Matrix(Shape *shape_, std::vector<Number> *data_)
    : shape{shape_}, data{data_} {}

Matrix::Matrix(std::vector<Number> *data_) : data{data_} {
    shape = new Shape{(unsigned int)data->size()};
}

Matrix::~Matrix() {
    delete shape;
    delete data;
}

bool Matrix::is_vector() const {
    return shape->size() == 1 && shape->at(0) > 1;
}

bool Matrix::is_scalar() const {
    return shape->size() == 1 && shape->at(0) == 1;
}

bool Matrix::is_integer() const {
    return is_scalar() && (*data)[0].type == NumInteger;
}

bool Matrix::is_real() const {
    return is_scalar() && (*data)[0].type == NumReal;
}

bool Matrix::is_complex() const {
    return is_scalar() && (*data)[0].type == NumReal;
}

bool Matrix::is_nil() const {
    return shape == nullptr;
}

Number &Matrix::at(unsigned long index) const {
    return data->at(index);
}

Shape *Matrix::get_shape() {
    return shape;
}

unsigned long Matrix::count() const {
    return data->size();
}

unsigned long Matrix::to_shape(Shape *s) const {
    unsigned long size = 1;
    for (unsigned long i = 0; i < count(); i++) {
        Number n = at(i);
        if (!n.is_integer() || n.integer < 0) {
            throw "domain error";
        }
        s->push_back(n.integer);
        size *= n.integer;
    }
    return size;
}

std::ostream &operator<<(std::ostream &os, const Matrix &matrix) {
    if (matrix.is_nil()) {
        return os;
    } else if (matrix.is_scalar()) {
        os << matrix.at(0) << std::endl;
    } else if (matrix.is_vector()) {
        for (unsigned long i = 0; i < matrix.count(); i++) {
            os << matrix.at(i);
            if (i < matrix.count() - 1) {
                os << " ";
            } else {
                os << std::endl;
            }
        }
    } else {

        // HACK: feels like there's a better solution to formatted
        // output. It finds the maxlength of the matrix, which is
        // passed to setw for each of its values.
        unsigned long maxlen = matrix.at(0).length();
        for (unsigned long i = 0; i < matrix.count(); i++) {
            if (matrix.at(i).length() > maxlen) {
                maxlen = matrix.at(i).length();
            }
        }

        unsigned long cols = matrix.shape->at(matrix.shape->size() - 1);
        unsigned long rows = matrix.shape->at(matrix.shape->size() - 2);
        for (unsigned long j = 1; j <= matrix.count(); j++) {
            os << std::setw(maxlen) << matrix.at(j - 1);
            if (j % cols != 0) {
                os << " ";
            } else {
                if (j % (rows * cols) == 0) {
                    os << std::endl;
                }
                if (j - 1 != matrix.count() - 1) {
                    os << std::endl;
                }
            }
        }
    }

    return os;
}