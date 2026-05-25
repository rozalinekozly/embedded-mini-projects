// group 

#ifndef ILRD_HRD42_GROUP_HPP
#define ILRD_HRD42_GROUP_HPP

#include <vector>
#include "shape.hpp" // Shape API

namespace hrd42
{
class Group : public Shape // non copyable
{
public:
    explicit Group(Shape& shape);
    // using generated dtor
    
    Group& AddShape(Shape* shape);

    virtual Group& Draw() const;
    virtual Group& Move(const ilrd::Point& dest);
    virtual Group& Move(double x_offest, double y_offset);
    virtual Group& Rotate(double angle);
    virtual Group& Revolve(const ilrd::Point& pivot, double angle);
    virtual Group& SetColor(COLORS color);
    
    virtual Group* Clone() const;

private:
    std::vector<Shape*> m_shapes;
};

} // namespace hrd42

#endif // ILRD_HRD42_GROUP_HPP