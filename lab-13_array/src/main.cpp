#include "my_array.h"
#include <cstddef>
#include <iostream>

using lab_13::my_array;

template<typename T, std::size_t N>
void test_core() {
    my_array<T, N> a;
    const my_array<T, N> &ca = a;
    a.empty();
    ca.empty();
    a.size();
    ca.size();

    a.at(0);
    ca.at(0);
    a[0];
    ca[0];
    (void) static_cast<const T &>(a[0]);
    (void) static_cast<const T &>(ca[0]);
}

template<typename T, std::size_t N>
void test_assign() {
    my_array<T, N> a;
    const my_array<T, N> &ca = a;
    a.fill(T());

    a[0] = T();
    a[0] = a[0];
    a[0] = ca[0];
}

class NonCopyable {
public:
    NonCopyable() {}

private:
    NonCopyable(const NonCopyable &);

    NonCopyable &operator=(const NonCopyable);
};

int main() {
    test_core<int, 10>();
    test_core<bool, 10>();
    test_core<NonCopyable, 10>();

    test_assign<int, 10>();
    test_assign<bool, 10>();

    const std::size_t size_array = 10;
    lab_13::my_array<bool, size_array> array_bool;
    array_bool.fill(true);

    const std::size_t assert_size = (size_array / 8) + (size_array % 8 != 0);

    const std::size_t actualy_size = sizeof(array_bool);

    std::cout << "size_array = " << size_array << std::endl;
    std::cout << "assert_size = " << assert_size << std::endl;
    std::cout << "actualy_size = " << actualy_size << std::endl;

    array_bool.at(0) = true;
    array_bool.at(1) = false;
    std::cout << array_bool[0] << std::endl;
    std::cout << array_bool.at(1) << std::endl;
}
