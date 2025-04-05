#include "Rectangle.hpp"
#include <iostream>

Rectangle::Rectangle(int id, int x, int y, int width, int height) : Figure(id, x, y), width_(width), height_(height) {}

bool Rectangle::is_inside(int x, int y) const
{
    return (x_ - width_ / 2 <= x && x <= x_ + width_ / 2) && (y_ - height_ / 2 <= y && y <= y_ + height_ / 2);
}
void Rectangle::zoom(int factor)
{
    width_ *= factor;
    height_ *= factor;
}

void Rectangle::print() const
{
    std::cout << "Rectangle ";
    Figure::print();
    std::cout << "width = " << width_ << " height = " << height_ << std::endl;
}
