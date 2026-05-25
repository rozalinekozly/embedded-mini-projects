// Based on composite design pattern of Shape
// circle 

#ifndef ILRD_HRD42_CIRCLE_HPP
#define ILRD_HRD42_CIRCLE_HPP

#include "shape.hpp" // Shape API

namespace hrd42
{
class Circle : public Shape // non copyable
{
public:
    explicit Circle(const ilrd::Point& center,
                    double radius = 1,
                    double angle = 0,
                    COLORS color = COLOR_WHITE);
    // using generated dtor

    virtual Circle& Draw() const;
    virtual Circle* Clone() const;

private:
    double m_radius;
};

} // namespace hrd42

#endif // ILRD_HRD42_CIRCLE_HPP