#include <iostream>
#include <cmath> 
#include "point.hpp"

namespace hrd42
{
Point::Point(int x, int y)
{
    this->m_x = x;
    this->m_y = y;
}
void Point::AdjustBy(Point p)
{
    this->m_x += p.m_x;
    this->m_y += p.m_y;
}
float Point::Length()
{
    float x_square = this->m_x *  this->m_x;
    float y_square = this->m_y * this->m_y;
     
    return (std::sqrt((x_square + y_square)));
}
void Point::Print(char first_parantheses, char second_parantheses)
{
    std::cout<< first_parantheses << this->m_x << "," 
    << this->m_y << second_parantheses <<std::endl;
}

Point Add(Point p1, Point p2)
{
    Point ret;
    ret.m_x = p1.m_x + p2.m_x;
    ret.m_y = p1.m_y + p2.m_y;

    return ret;

}

}//namespace hrd42