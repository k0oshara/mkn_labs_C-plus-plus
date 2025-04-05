#include "linq.hpp"
#include <cassert>

using linq::from;

void test_from() {
    int v[] = {1, 2, 3};
    std::vector<int> res = from(v, v + 3).to_vector();
    std::vector<int> expected = {1, 2, 3};
    assert(res == expected);
}

void test_select() {
    int v[] = { 1, 2, 3 };
    std::vector<int> res = from(v, v + 3).select([](int x) { return x + 5; }).to_vector();
    std::vector<int> expected = {6, 7, 8};
    assert(res == expected);
}

void test_drop() {
    int v[] = { 1, 2, 3 };
    std::vector<int> res = from(v, v + 3).drop(1).to_vector();
    std::vector<int> expected = {2, 3};
    assert(res == expected);

    std::vector<int> res2 = from(v, v + 3).drop(5).to_vector();
    std::vector<int> expected2 = {};
    assert(res2 == expected2);
}

void test_take() {
    int v[] = { 1, 2, 3 };
    std::vector<int> res = from(v, v + 3).take(2).to_vector();
    std::vector<int> expected = {1, 2};
    assert(res == expected);
}

void test_until() {
    int v[] = { 1, 2, 3 };
    std::vector<int> res = from(v, v + 3).until([](int x) {return x == 3; }).to_vector();
    std::vector<int> expected = {1, 2};
    assert(res == expected);
}

void test_until_eq() {
    int v[] = { 1, 2, 3 };
    std::vector<int> res = from(v, v + 3).until_eq(2).to_vector();
    std::vector<int> expected = {1};
    assert(res == expected);
}

void test_where() {
    int v[] = { 1, 2, 3 };
    std::vector<int> res = from(v, v + 3).where([](int x) {return x*x > 1;}).to_vector();
    std::vector<int> expected = {2, 3};
    assert(res == expected);
}

void test_where_neq() {
    int v[] = { 1, 2, 3 };
    std::vector<int> res = from(v, v + 3).where_neq(2).to_vector();
    std::vector<int> expected = {1, 3};
    assert(res == expected);
}

void tests() {
    test_from();
    test_select();
    test_drop();
    test_take();
    test_until();
    test_until_eq();
    test_where();
    test_where_neq();
}

int main() {
    tests();
}
