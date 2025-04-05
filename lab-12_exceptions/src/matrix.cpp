#include "matrix.h"
#include <stdexcept>
#include <fstream>

namespace matrix {

    void Matrix::swap(Matrix &a, Matrix &b) {
        std::swap(a._rows, b._rows);
        std::swap(a._cols, b._cols);
        std::swap(a._data, b._data);
    }

    Matrix::Matrix() : _rows(0), _cols(0), _data(nullptr) {}

    Matrix::Matrix(const std::string &filename) {

        std::ifstream file(filename);

        if (!file.is_open()) {
            throw MatrixException("LOAD: unable to open file.");
        }
        file >> _rows;
        if (file.fail() || file.bad()) {
            throw MatrixException("LOAD: invalid file format.");
        }
        file >> _cols;
        if (file.fail() || file.bad()) {
            throw MatrixException("LOAD: invalid file format.");
        }

        Matrix tmp(_rows, _cols);
        for (std::size_t i = 0; i < _rows; ++i) {
            for (std::size_t j = 0; j < _cols; ++j) {
                int x = 0;
                file >> x;
                if (file.fail() || file.bad()) {
                    throw MatrixException("LOAD: invalid file format.");
                }
                tmp._data[i][j] = x;
            }
        }
        swap(*this, tmp);

    }

    Matrix::Matrix(std::size_t r, std::size_t c) {
        _rows = r;
        _cols = c;
        int *mem = nullptr;
        try {
            _data = new int *[_rows];
            mem = new int[_rows * _cols];
        }
        catch (std::bad_alloc &e) {
            delete[]mem;
            throw e;
        }

        for (std::size_t i = 0; i < _rows; ++i) {
            _data[i] = &mem[i * _cols];
        }
        for (std::size_t i = 0; i < _rows; ++i) {
            for (std::size_t j = 0; j < _cols; ++j) {
                _data[i][j] = 0;
            }
        }
    }

    Matrix::Matrix(const Matrix &m) : Matrix(m._rows, m._cols) {
        for (std::size_t i = 0; i < m._rows; ++i) {
            for (std::size_t j = 0; j < m._cols; ++j) {
                _data[i][j] = m._data[i][j];
            }
        }
    }

    Matrix::~Matrix() {
        if (_data != nullptr) {
            delete[] _data[0];
        }
        delete[] _data;
    }

    Matrix Matrix::operator+(const Matrix &m) const{
        if (_rows != m._rows || _cols != m._cols) {
            throw MatrixException("ADD: dimensions do not match.");
        }
        Matrix result(*this);
        for (size_t i = 0; i < _rows; i++) {
            for (size_t j = 0; j < _cols; j++) {
                result._data[i][j] = _data[i][j] + m._data[i][j];
            }
        }
        return result;
    }

    Matrix Matrix::operator*(const Matrix &m) const {
        if (_cols != m._rows) {
            throw MatrixException("MUL: #arg1.columns != #arg2.rows.");
        }

        Matrix tmp(_rows, m._cols);
        for (std::size_t i = 0; i < _rows; ++i) {
            for (std::size_t j = 0; j < m._cols; ++j) {
                for (std::size_t k = 0; k < _cols; ++k) {
                    tmp._data[i][j] += _data[i][k] * m._data[k][j];
                }
            }
        }
        return tmp;
    }

    Matrix &Matrix::operator+=(const Matrix &m) {
        if (_rows != m._rows || _cols != m._cols) {
            throw MatrixException("ADD: dimensions do not match.");
        }

        *this = *this + m;
        return *this;
    }


    Matrix &Matrix::operator*=(const Matrix &m) {
        if (_cols != m._rows) {
            throw MatrixException("MUL: #arg1.columns != #arg2.rows.");
        }

        *this = *this * m;
        return *this;
    }

    Matrix &Matrix::operator=(const Matrix &m) {
        if (this == &m) {
            return *this;
        }
        Matrix temp(m);
        swap(*this, temp);
        return *this;
    }

    int Matrix::get(size_t i, size_t j) const {
        if (i < 0 || i >= _rows || j >= _cols || j < 0) {
            throw MatrixException("ACCESS: bad index.");
        }

        return _data[i][j];
    }

    void Matrix::print(std::ostream &os) const {
        for (size_t i = 0; i < _rows; i++) {
            for (size_t j = 0; j < _cols - 1; j++) {
                os << _data[i][j] << ' ';
            }
            os << _data[i][_cols - 1] << std::endl;
        }
    }
}
