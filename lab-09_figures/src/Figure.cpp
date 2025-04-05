#include "Figure.hpp"
#include <cstdio>
#include <iostream>

Figure::Figure(int id, int x, int y) : id_(id), x_(x), y_(y){}

void Figure::move(int new_x, int new_y)
{
    x_ = new_x;
    y_ = new_y;
}

int Figure::get_id() const {
    return id_;
}
void Figure::print() const {
    std::cout << id_ << ": x = " << x_ << " y = " << y_ << " ";
}
