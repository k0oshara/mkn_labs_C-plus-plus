#ifndef MY_ARRAY_H_
#define MY_ARRAY_H_

#include <cstddef>
#include <stdexcept>
#include <cstdint>

namespace lab_13 {

    template<typename T, std::size_t N>
    class my_array {
    private:
        T _data[N] {};
    public:
        const T &at(std::size_t index) const;

        T &at(std::size_t index);

        const T &operator[](std::size_t index) const;

        T &operator[](std::size_t index);

        bool empty() const;

        std::size_t size() const;

        void fill(const T &val);
    };

    template<typename T, std::size_t N>
    void my_array<T, N>::fill(const T &val) {
        for (std::size_t i = 0; i < N; ++i) {
            _data[i] = val;
        }
    }

    template<typename T, std::size_t N>
    std::size_t my_array<T, N>::size() const {
        return N;
    }

    template<typename T, std::size_t N>
    bool my_array<T, N>::empty() const {
        return N == 0;
    }

    template<typename T, std::size_t N>
    const T &my_array<T, N>::operator[](std::size_t index) const {
        return _data[index];
    }

    template<typename T, std::size_t N>
    T &my_array<T, N>::operator[](std::size_t index) {
        return _data[index];
    }

    template<typename T, std::size_t N>
    const T &my_array<T, N>::at(std::size_t index) const {
        if (index >= N) {
            throw std::out_of_range("Index out of range");
        }
        return _data[index];
    }

    template<typename T, std::size_t N>
    T &my_array<T, N>::at(std::size_t index) {
        if (index >= N) {
            throw std::out_of_range("Index out of range");
        }
        return _data[index];
    }

    class Proxy {
    private:
        uint8_t& _byte;
        uint8_t _mask;
    public:
        Proxy(uint8_t &byte, uint8_t mask) : _byte(byte), _mask(mask) {}

        Proxy& operator=(const Proxy &val) {
            if (*this == val) {
                return *this;
            }

            if (val) {
                _byte |= _mask;
            } else {
                _byte &= ~_mask;
            }

            return *this;
        }

        Proxy& operator=(bool val) {

            if (val) {
                _byte |= _mask;
            } else {
                _byte &= ~_mask;
            }

            return *this;
        }

        operator bool() const {
            return _byte & _mask;
        }
    };

    template<std::size_t N>
    class my_array <bool, N> {
    private:
        uint8_t _data[(N + 7) / 8]{};
    public:

        bool at(std::size_t index) const;

        Proxy at(std::size_t index);

        bool operator[](std::size_t index) const;

        Proxy operator[](std::size_t index);

        bool empty() const;

        std::size_t size() const;

        void fill(const bool &val);
    };

    template<std::size_t N>
    void my_array<bool, N>::fill(const bool &val) {
        uint8_t fill_value = val ? 0xFF : 0x00;
        for (std::size_t i = 0; i < (N + 7) / 8; i++) {
            _data[i] = fill_value;
        }
    }

    template<std::size_t N>
    std::size_t my_array<bool, N>::size() const {
        return N;
    }

    template<std::size_t N>
    bool my_array<bool, N>::empty() const {
        return N == 0;
    }

    template<std::size_t N>
    bool my_array<bool, N>::operator[](std::size_t index) const {
        return _data[index / 8] & (1 << (index % 8));
    }

    template<std::size_t N>
    Proxy my_array<bool, N>::operator[](std::size_t index) {
        return Proxy(_data[index / 8], 1 << (index % 8));
    }

    template<std::size_t N>
    bool my_array<bool, N>::at(std::size_t index) const {
        if (index >= N) {
            throw std::out_of_range("Index out of range");
        }
        return _data[index / 8] & (1 << (index % 8));
    }

    template<std::size_t N>
    Proxy my_array<bool, N>::at(std::size_t index) {
        if (index >= N) {
            throw std::out_of_range("Index out of range");
        }
        return Proxy(_data[index / 8], 1 << (index % 8));
    }

}  // namespace lab_13

#endif  // MY_ARRAY_H_
