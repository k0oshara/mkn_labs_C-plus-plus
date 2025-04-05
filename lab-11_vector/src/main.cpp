#include "my_vector.hpp"

#include <iostream>
#include <cstring>
#include <cassert>
#include <ostream>
#include <random>

namespace product
{
    class Product
    {
    public:
        Product() : quantity_(0), price_(0)
        {
            name_ = new char[0];
        }
        Product(const char* name, int quantity, double price) : quantity_(quantity), price_(price)
        {
            name_ = new char[strlen(name) + 1];
        }

        Product(const Product& other)
        {
            name_ = new char[strlen(other.name_) + 1];
            strcpy(name_, other.name_);

            quantity_ = other.quantity_;
            price_ = other.price_;
        }

        ~Product()
        {
            delete[] name_;
        }

        Product& operator=(const Product& other)
        {
            if (this == &other)
            {
                return *this;
            }

            delete[] name_;

            name_ = new char[strlen(other.name_) + 1];
            strcpy(name_, other.name_);

            quantity_ = other.quantity_;
            price_ = other.price_;

            return *this;

        }

        bool operator==(const Product& other) const
        {
            return strcmp(name_, other.name_) == 0 && quantity_ == other.quantity_ && price_ == other.price_;
        }

        friend std::ostream& operator<<(std::ostream& os, const Product& p)
        {
            return os << p.name_ << " " << p.quantity_ << " " << p.price_;
        }

    private:
        char *name_;
        int quantity_;
        double price_;
    };
}

template<typename T>
void test_my_vector(T* array, const size_t size)
{
    containers::my_vector<T> v;

    for (size_t i = 0; i < size; i++)
    {
        v.push_back(array[i]);
    }

    assert(v.size() == size);

    for (size_t i = 0; i < size; ++i)
    {
        assert(v[i] == array[i]);
    }

    size_t min_power_of_two_to_10 = 1;
    while (min_power_of_two_to_10 < size)
    {
        min_power_of_two_to_10 *= 2;
    }

    assert(v.capacity() == min_power_of_two_to_10);

    assert(!v.empty());

    for (size_t i = 0; i < size; ++i)
    {
        assert(v.size() == size - i);
        assert(v.capacity() == min_power_of_two_to_10);
        v.pop_back();
    }

    assert(v.size() == 0);
    assert(v.capacity() == min_power_of_two_to_10);

    assert(v.empty());

    for (size_t i = size - 1; i > size; i--)
    {
        v.push_back(array[i]);
    }

    containers::my_vector<T> v2(v);
    assert(v.size() == v2.size());
    assert(v.capacity() == v2.capacity());
    assert(v == v2);

    containers::my_vector<T> v3;
    v3 = v;
    assert(v.size() == v3.size());
    assert(v.capacity() == v3.capacity());
    assert(v == v3);

    v3.resize(7);
    assert(v3.size() == 7);
    v3.reserve(31);
    assert(v3.capacity() == 32);

    v3.reserve(33);
    assert(v3.capacity() == 64);

}

int main() {
	containers::my_vector<int> v;
	v.push_back(2);
	const int n{3};
	v.push_back(n);

    assert(v[0] == 2);
    assert(v[1] == 3);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(1, 1000000);


    int array[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    size_t size_array = sizeof(array) / sizeof(array[0]);

    for (size_t i = 0; i < size_array; ++i)
    {
        array[i] = distrib(gen);
    }

    product::Product porduct_array[]
    {
        product::Product("asdf", 4, 12.0),
        product::Product("qwe", -1, 7.5),
        product::Product("ryt", 4, 12.0),
        product::Product("qwuuyte", -1, 7.5),
        product::Product("opg", 4, 12.0),
        product::Product("zxvc", -1, 7.5),
        product::Product("asd", 4, 12.0),
        product::Product("qwgb", -1, 7.5),
        product::Product("assfdhdf", 4, 12.0),
        product::Product("bsdfs", -1, 7.5)
    };

    test_my_vector<int>(array, sizeof(array) / sizeof(array[0]));
    test_my_vector<product::Product>(porduct_array, sizeof(porduct_array) / sizeof(porduct_array[0]));

    return 0;
}
