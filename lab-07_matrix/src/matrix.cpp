#include "matrix.h"
#include <stdexcept>
#include <cstring>

Matrix::Matrix(size_t r, size_t c)
{
    _rows = r;
    _cols = c;

    _data = new int*[_rows];
    for (size_t i = 0; i < _rows; i++)
    {
        _data[i] = new int[_cols];
        for (size_t j = 0; j < _cols; j++)
        {
            _data[i][j] = 0;
        }
    }
}

void Matrix::my_copy(const Matrix& m)
{
    _rows = m._rows;
    _cols = m._cols;
    _data = new int*[_rows];
    for (size_t i = 0; i < _rows; i++)
    {
        _data[i] = new int[_cols];
        memcpy(_data[i], m._data[i], _cols * sizeof(int));
    }
}

Matrix::Matrix(const Matrix& m)
{
    my_copy(m);
}

Matrix& Matrix::operator=(const Matrix& that){
    if (this == &that){
        return *this;
    }
    for (size_t i = 0; i < _rows; i++)
    {
        delete[] _data[i];
    }
    delete[] _data;
    my_copy(that);
    return *this;
}

Matrix::~Matrix()
{
    for (size_t i = 0; i < _rows; i++)
    {
        delete[] _data[i];
    }
    delete[] _data;
}

size_t Matrix::get_rows() const
{
    return _rows;
}
size_t Matrix::get_cols() const
{
    return _cols;
}

void Matrix::set(size_t i, size_t j, int val)
{
    if (i >= _rows || j >= _cols){
        throw std::out_of_range("Index out of range");
    }
    _data[i][j] = val;
}

int Matrix::get(size_t i, size_t j) const
{
    if (i >= _rows || j >= _cols){
        throw std::out_of_range("Index out of range");
    }
    return _data[i][j];
}

void Matrix::print(FILE *f) const
{
    for (size_t i = 0; i < _rows; i++)
    {
        for (size_t j = 0; j < _cols - 1; j++)
        {
            fprintf(f, "%d ", _data[i][j]);
        }
        fprintf(f, "%d\n", _data[i][_cols - 1]);
    }
}

Matrix Matrix::matrix_realize_operator(Matrix& m, std::function<int(int, int)> operation)
{
    Matrix result(*this);
    for (size_t i = 0; i < _rows; i++)
    {
        for (size_t j = 0; j < _cols; j++)
        {
            result._data[i][j] = operation(_data[i][j], m._data[i][j]);
        }
    }
    return result;

}

Matrix Matrix::operator+(Matrix& m)
{
    return matrix_realize_operator(m, [](int a, int b) { return a + b; });
}

Matrix& Matrix::operator+=(Matrix& m)
{
    Matrix res = *this + m;
    *this = res;
    return *this;
}

Matrix Matrix::operator-(Matrix& m)
{
    return matrix_realize_operator(m, [](int a, int b) { return a - b; });
}

Matrix& Matrix::operator-=(Matrix& m)
{
    Matrix res = *this - m;
    *this = res;
    return *this;
}

Matrix Matrix::operator*(Matrix& m)
{
    if (_cols != m._rows){
        throw std::invalid_argument("Matrix dimensions do not match");
    }
    Matrix result(_rows, m._cols);

    for (size_t i = 0; i < _rows; i++)
    {
        for (size_t j = 0; j < m._cols; j++)
        {
            result._data[i][j] = 0;
            for (size_t k = 0; k < _cols; k++)
            {
                result._data[i][j] += _data[i][k] * m._data[k][j];
            }
        }
    }
    return result;
}

Matrix& Matrix::operator*=(Matrix& m)
{
    if (_cols != m._rows){
        throw std::invalid_argument("Matrix dimensions do not match");
    }
    Matrix res = *this * m;
    *this = res;
    return *this;
}

bool Matrix::operator==(const Matrix& m) const
{
    for (size_t i = 0; i < _rows; i++)
    {
        for (size_t j = 0; j < _cols; j++)
        {
            if (_data[i][j] != m._data[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

bool Matrix::operator!=(const Matrix& m) const
{
    return !(*this == m);
}
