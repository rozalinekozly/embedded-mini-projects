#include "shape.hpp"
#include "circle.hpp"

using namespace ilrd;
using namespace hrd42;

Circle::Circle(const ilrd::Point& center,
                    double radius ,
                    double angle,
                    COLORS color)  : Shape(center, angle, color)
{
    this->m_radius = radius;

}

double Circle::GetRadius() const
{
    return this->m_radius;
}

Circle* Circle::Clone() const
{
    //TODO: can just call the cctor (unblock it from shape api)
   /* return new Circle(this->GetPoint(), this->GetRadius(),
                    this->GetAngle(), this->GetColor());*/
    return (new Circle(*this));
}


Circle& Circle:: Draw() const
{
    DrawCircle(this->GetColor(), this->GetPoint().GetX(), this->GetPoint().GetY(), this->GetRadius());
    // added in order to support returning a reference from const method
    return const_cast<Circle&>(*this);
}

