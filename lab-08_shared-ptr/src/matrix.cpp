#include "matrix.hpp"
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

Matrix::Matrix(const Matrix& m)
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

Matrix& Matrix::operator=(const Matrix& that){
    if (this == &that){
        return *this;
    }
    for (size_t i = 0; i < _rows; i++)
    {
        delete[] _data[i];
    }
    delete[] _data;
    _rows = that._rows;
    _cols = that._cols;
    _data = new int*[_rows];
    for (size_t i = 0; i < _rows; i++)
    {
        _data[i] = new int[_cols];
        memcpy(_data[i], that._data[i], _cols * sizeof(int));
    }
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
