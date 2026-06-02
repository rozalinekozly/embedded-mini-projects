// Based on composite design pattern of Shape
// square 

#ifndef ILRD_HRD42_SQUARE_HPP
#define ILRD_HRD42_SQUARE_HPP

#include "shape.hpp" // Shape API

namespace hrd42
{
class Square : public Shape // non copyable
{
public:
    explicit Square(const ilrd::Point& center,
                  double width = 1,
                  double angle = 0,
                  COLORS color = COLOR_WHITE);
    // using generated dtor

    virtual Square& Draw() const;
    virtual Square* Clone() const;
    
    double GetWidth() const;
private:
    double m_width;
};

} // namespace hrd42

#endif // ILRD_HRD42_SQUARE_HPP