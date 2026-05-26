#include "shape.hpp"
#include "circle.hpp"

using namespace ilrd;
using namespace hrd42;

Circle::Circle(const ilrd::Point& center,
                    double radius = 1,
                    double angle = 0,
                    COLORS color = COLOR_WHITE)  : Shape(center, angle, color)
{
    this->m_radius = radius;

}

double Circle::GetRadius() const
{
    return this->m_radius;
}

Circle* Circle::Clone() const
{
    //can just call the cctor 
    return new Circle(this->GetPoint(), this->GetRadius(),
                    this->GetAngle(), this->GetColor());
}


Circle& Circle:: Draw() const
{
    DrawCircle(this->GetColor(), this->GetPoint().GetX(), this->GetPoint().GetY(), this->GetRadius());
    // added in order to support returning a reference from const method
    return const_cast<Circle&>(*this);
}

