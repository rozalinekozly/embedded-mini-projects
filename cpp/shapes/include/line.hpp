// Based on composite design pattern of Shape
// line 

#ifndef ILRD_HRD42_LINE_HPP
#define ILRD_HRD42_LINE_HPP

#include "shape.hpp" // Shape API

namespace hrd42
{
class Line : public Shape // non copyable
{
public:
    explicit Line(const ilrd::Point& center,
                  double length = 1,
                  double angle = 0,    
                  COLORS color = COLOR_WHITE);
    // using generated dtor

    virtual Line& Draw() const;
    virtual Line* Clone() const;

    double GetLength() const;
private:
    double m_length;
};

} // namespace hrd42

#endif // ILRD_HRD42_LINE_HPP