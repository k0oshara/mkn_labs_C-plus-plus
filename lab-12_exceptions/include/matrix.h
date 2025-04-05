#pragma once

#include <cstdio>
#include <cstddef>
#include <string>
#include <exception>

namespace matrix {

    class MatrixException : public std::exception {
    private:
        std::string message_;
    public:
        MatrixException(std::string ex) : message_(std::move(ex)) {}

        const char *what() const

        noexcept
        {
            return message_.c_str();
        }
    };

    class Matrix {
    public:
        Matrix();

        Matrix(const std::string &filename);

        Matrix(std::size_t r, std::size_t c);

        Matrix(const Matrix &m);

        ~Matrix();

        Matrix operator+(const Matrix &m) const;

        Matrix operator*(const Matrix &m) const;

        Matrix &operator=(const Matrix &m);

        Matrix &operator+=(const Matrix &m);

        Matrix &operator*=(const Matrix &m);

        int get(size_t i, size_t j) const;

        void print(std::ostream &os) const;

    private:
        static void swap(Matrix &a, Matrix &b);

        std::size_t _rows{};
        std::size_t _cols{};
        int **_data{};

    };
}
