#include <iostream>
#include <cmath> 
#include "point.hpp"

namespace hrd42
{
void Point::AdjustBy(Point p)
{
    this->m_x += p.m_x;
    this->m_y += p.m_y;
}
double Point::Length()
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
}//namespace hrd42