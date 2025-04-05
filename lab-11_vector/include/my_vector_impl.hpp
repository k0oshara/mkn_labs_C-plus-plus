#ifndef LAB_11_VECTOR_MY_VECTOR_IMPL_H
#define LAB_11_VECTOR_MY_VECTOR_IMPL_H

#include <cstddef>
#include <stdexcept>
#include <ostream>

namespace containers
{
    template<typename T>
    void my_vector<T>::increase_capacity(std::size_t const n)
    {
        while (capacity_ < n)
        {
            capacity_ *= 2;
        }
    }

    template<typename E>
    std::ostream &operator<<(std::ostream &os, const my_vector<E> &v) {
        for (size_t i = 0; i < v.size() - 1; i++)
        {
            os << v.array_[i] << " ";
        }
        os << v[v.size() - 1];

        return os;
    }

    template<typename T>

    my_vector<T>::my_vector() {
        size_ = 0;
        capacity_ = 1;
        array_ =(T*) new char[capacity_ * sizeof(T)];
    }

    template<typename T>
    my_vector<T>::my_vector(std::size_t n)
    {
        size_ = n;
        capacity_ = 1;

        increase_capacity(n);

        array_ = (T*) new char[capacity_ * sizeof(T)];
        for (size_t i = 0; i < size_; ++i)
        {
            new(array_ + i) T();
        }
    }

    template<typename T>
    my_vector<T>::my_vector(const my_vector &other)
    {
        size_ = other.size_;
        capacity_ = other.capacity_;
        array_ = (T*)new char[capacity_ * sizeof(T)];

        for (size_t i = 0; i < size_; ++i)
        {
            new(array_ + i) T(other.array_[i]);
        }
    }

    template<typename T>
    my_vector<T>::~my_vector()
    {
        for (size_t i = 0; i < size_; ++i)
        {
            array_[i].~T();
        }
        delete[] (char*) array_;
    }

    template<typename T>
    my_vector<T> &my_vector<T>::operator=(my_vector<T> other)
    {
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
        std::swap(array_, other.array_);

        return *this;
    }

    template<typename T>
    T& my_vector<T>::operator[](std::size_t index) const
    {
        if (index >= size_)
        {
            throw std::out_of_range("Index is out of range");
        }
        return array_[index];
    }

    template<typename T>
    std::size_t my_vector<T>::size() const
    {
        return size_;
    }

    template<typename T>
    std::size_t my_vector<T>::capacity() const
    {
        return capacity_;
    }

    template<typename T>
    bool my_vector<T>::empty() const
    {
        return size_ == 0;
    }

    template<typename T>
    void my_vector<T>::resize(std::size_t n)
    {
        if (n <= size_)
        {
            for (std::size_t i = n; i < size_; i++)
            {
                array_[i].~T();
            }
            size_ = n;
        }
        else
        {
            if (n > capacity_)
            {
                reserve(n);
            }

            for (std::size_t i = size_; i < n; i++)
            {
                new(array_ + i) T();
            }
            size_ = n;
        }

    }

    template<typename T>
    void my_vector<T>::reserve(std::size_t n)
    {
        if (n <= capacity_)
        {
            return;
        }

        increase_capacity(n);

        T *new_data = (T*)new char[capacity_ * sizeof(T)];

        for (std::size_t i = 0; i < size_; ++i)
        {
            new(new_data + i) T(array_[i]);
            array_[i].~T();
        }
        delete[] (char*)array_;
        array_ = new_data;
    }

    template<typename T>
    void my_vector<T>::push_back(const T &t)
    {
        if (size_ >= capacity_)
        {
            reserve(capacity_ * 2);
        }
        new(array_ + size_) T(t);
        size_++;
    }

    template<typename T>
    void my_vector<T>::pop_back()
    {
        if (size_ == 0)
        {
            throw std::out_of_range("Vector is empty");
        }
        array_[--size_].~T();
    }

    template<typename T>
    void my_vector<T>::clear()
    {
        while (size_ > 0)
        {
            pop_back();
        }
    }

    template<typename T>
    bool operator==(const my_vector<T> &a, const my_vector<T> &b)
    {
        bool flag = true;
        if (a.size() != b.size() || a.capacity() != b.capacity())
        {
            flag = false;
        }
        else if (a.size() == b.size() && a.capacity() == b.capacity())
        {
            for (size_t i = 0; i < a.size(); i++)
            {
                if (!(a[i] == b[i]))
                {
                    flag = false;
                }
            }
        }
        return flag;
    }
}
#endif
