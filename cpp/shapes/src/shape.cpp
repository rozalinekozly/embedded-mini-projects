#include "shape.hpp"
#include "point.hpp"

using namespace ilrd;
using namespace hrd42;

Shape::Shape(const ilrd::Point& center, double angle, COLORS color)
{
    this->m_center = center;
    this->m_angle = angle;
    this->m_color = color;
}

Shape::~Shape() {}

Shape& Shape::Move(const ilrd::Point& dest)
{
    this->m_center.Move(dest.GetX(), dest.GetY());
    return (*this);
}

Shape& Shape::Move(double x_offest, double y_offset)
{
    Step(&(this->m_center), x_offest, y_offset);
    return (*this);
}

Shape& Shape::Rotate(double angle)
{
    this->m_angle += angle;
    return (*this);
}

Shape& Shape::Revolve(const ilrd::Point& pivot, double angle)
{
    this->m_center = this->m_center.Revolve(pivot, angle);
    return (this->Rotate(angle));
}


Shape& Shape::SetColor(COLORS color)
{
    this->m_color = color;
    return (*this);
}

COLORS Shape:: GetColor() const
{
    return (this->m_color);
}

double Shape::GetAngle() const
{
    return (this->m_angle);
}

Point Shape::GetPoint() const
{
    return (this->m_center);
}

