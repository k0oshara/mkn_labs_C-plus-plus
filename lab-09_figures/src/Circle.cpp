#include "Circle.hpp"
#include <iostream>
#include <cstring>

Circle::Circle(int id, int x, int y, int radius, const char* label) : Figure(id, x, y), radius_(radius)
{
    label_ = new char[strlen(label) + 1];
    strcpy(label_, label);
}

Circle::~Circle()
{
    delete[] label_;
}

bool Circle::is_inside(int x, int y) const
{
    return (x - x_) * (x - x_) + (y - y_) * (y - y_) <= radius_ * radius_;
}

void Circle::zoom(int factor)
{
    radius_ *= factor;
}

void Circle::print() const
{
    std::cout << "Circle ";
    Figure::print();
    std::cout << "radius = " << radius_ << " label = " << label_ << std::endl;
}
