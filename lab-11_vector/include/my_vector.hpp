#ifndef MY_VECTOR_H_
#define MY_VECTOR_H_

#include <cstddef>
#include <ostream>

namespace containers
{

template<typename T>
    class my_vector
    {
    public:
        my_vector();
        explicit my_vector(std::size_t n);
        my_vector(my_vector const &other);
        my_vector& operator=(my_vector other);
        ~my_vector();

        std::size_t size() const;
        std::size_t capacity() const;
        bool empty() const;

        void resize(std::size_t n);
        void reserve(std::size_t n);

        T& operator[](std::size_t index) const;

        void push_back(const T &t);
        void pop_back();
        void clear();
        void increase_capacity(std::size_t const n);

        template<typename E>
        friend std::ostream & operator<<(std::ostream &os, const my_vector<E> &v);

    private:
        size_t capacity_;
        size_t size_;
        T* array_;
    };

    template<typename T>
    bool operator==(const my_vector<T> &a, const my_vector<T> &b);

}

#include "my_vector_impl.hpp"

#endif  // MY_VECTOR_H_
