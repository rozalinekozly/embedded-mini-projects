// Based on composite design pattern of Shape
// rectangle 

#ifndef ILRD_HRD42_RECTANGLE_HPP
#define ILRD_HRD42_RECTANGLE_HPP

#include "shape.hpp" // Shape API

namespace hrd42
{
class Rectangle : public Shape // non copyable
{
public:
    explicit Rectangle(const ilrd::Point& center,
                       double width = 1, double height = 1,
                       double angle = 0,
                       COLORS color = COLOR_WHITE           );
    // using generated dtor

    virtual Rectangle& Draw() const;
    virtual Rectangle* Clone() const;

private:
    double m_width;
    double m_height;
};

} // namespace hrd42

#endif // ILRD_HRD42_RECTANGLE_HPP
