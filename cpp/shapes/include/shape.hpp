// shape 
#ifndef ILRD_HRD42_SHAPE_HPP
#define ILRD_HRD42_SHAPE_HPP

namespace hrd42
{
class Shape // non copyable
{
public:
    explicit Shape(const ilrd::Point& center,
                   double angle = 0,
                   COLORS color = COLOR_WHITE);
    virtual ~Shape();
    
    virtual Shape& Draw() const = 0;
    virtual Shape& Move(const ilrd::Point& dest);
    virtual Shape& Move(double x_offest, double y_offset);
    virtual Shape& Rotate(double angle);
    virtual Shape& Revolve(const ilrd::Point& pivot, double angle);
    virtual Shape& SetColor(COLORS color);
    
    virtual Shape* Clone() const = 0; // pure?

private:
    ilrd::Point m_center;
    double m_angle;
    COLORS m_color;
    
    Shape(const Shape& other);
    Shape& operator=(const Shape& other);
};

} // namespace hrd42

#endif // ILRD_HRD42_SHAPE_HPP