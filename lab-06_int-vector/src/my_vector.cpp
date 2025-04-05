#include "my_vector.hpp"
#include <cstring>
#include <algorithm>
#include <stdexcept>

MyVector::MyVector() : _size(0), _capacity(2), _data(new int[2]) {}
MyVector::MyVector(std::size_t init_capacity) : _size(0), _capacity(init_capacity), _data(new int[init_capacity]) {}
MyVector::~MyVector() {
    delete[] _data;
}

MyVector::MyVector(const MyVector& that){
    _size = that._size;
    _capacity = that._capacity;
    _data = new int[_capacity];
    memcpy(_data, that._data, sizeof(int) * _size);
}

MyVector& MyVector::operator=(const MyVector& that){
    if (this == &that) {
        return *this;
    }
    delete[] _data;
    _size = that._size;
    _capacity = that._capacity;
    _data = new int[_capacity];
    memcpy(_data, that._data, sizeof(int) * _size);
    return *this;
}

void MyVector::set(std::size_t index, int value) {
    if (index < _size) {
        _data[index] = value;
    }
}

int MyVector::get(std::size_t index) {
    if (index < _size) {
        return _data[index];
    }
    throw std::out_of_range("Index out of range");
}

std::size_t MyVector::size() {
    return _size;
}

std::size_t MyVector::capacity() {
    return _capacity;
}

void MyVector::reserve(std::size_t new_capacity) {
    if (new_capacity <= _capacity) return;

    int* new_data = new int[new_capacity];
    std::memcpy(new_data, _data, sizeof(int) * _size);

    delete[] _data;
    _data = new_data;
    _capacity = new_capacity;
}

void MyVector::resize(std::size_t new_size) {
    if (new_size > _capacity) {
        reserve(std::max(new_size, _capacity * 2));
    }

    int* new_data = new int[new_size];
    for (std::size_t i = 0; i < new_size; i++) {
        if (i < _size) {
            new_data[i] = _data[i];
        } else {
            new_data[i] = 0;
        }
    }

    delete[] _data;
    _data = new_data;
    _size = new_size;
}

void MyVector::push_back(int value) {
    if (_size == _capacity) {
        reserve(2 * _capacity);
    }
    _data[_size++] = value;
}

void MyVector::insert(std::size_t index, int value) {
    if (_size == _capacity) {
        reserve(2 * _capacity);
    }

    std::memcpy(_data + index + 1, _data + index, (_size - index) * sizeof(int));
    _data[index] = value;
    _size++;
}

void MyVector::erase(std::size_t index) {
    if (index >= _size) {
        return;
    }
    std::memmove(_data + index, _data + index + 1, (_size - index - 1) * sizeof(int));
    _size--;
}
