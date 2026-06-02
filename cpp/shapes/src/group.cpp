#include "shape.hpp"
#include "group.hpp"

using namespace ilrd;
using namespace hrd42;


Group::Group(Shape& shape) : Shape(shape.GetPoint(), 0, shape.GetColor()) 
{
    this->m_shapes.push_back(&shape);
}

Group& Group::AddShape(Shape* shape)
{
    if(nullptr != shape)
    {
        this->m_shapes.push_back(shape);
    }
    return (*this);
}

Group& Group::Draw() const
{
    for(int i = 0 ; i < this->m_shapes.size() ; i++)
    {
        m_shapes[i]->Draw();
    }
    return const_cast <Group&>(*this);
}

Group& Group::Move(const ilrd::Point& dest)
{
    for(int i = 0 ; i < this->m_shapes.size() ; i++)
    {
        m_shapes[i]->Move(dest);
    }
    return const_cast <Group&>(*this);
}

Group& Group::Move(double x_offest, double y_offset)
{
   for(int i = 0 ; i < this->m_shapes.size() ; i++)
    {
        m_shapes[i]->Move(x_offest, y_offset);
    }
    return const_cast <Group&>(*this);
}

 Group& Group::Rotate(double angle)
 {
    //each element does a rotation around hs center, incorrect
    for(int i = 0 ; i < this->m_shapes.size() ; i++)
    {
        m_shapes[i]->Rotate(angle);
    }
    return const_cast <Group&>(*this);
 }
 
 Group& Group::Revolve(const ilrd::Point& pivot, double angle)
 {
    //Point group_pivot = this->m_shapes[0]->GetPoint();
    for(int i = 0 ; i < this->m_shapes.size() ; i++)
    {
        m_shapes[i]->Revolve(pivot, angle);
    }
    return const_cast <Group&>(*this);
 }

Group& Group::SetColor(COLORS color)
{
    for(int i = 0 ; i < this->m_shapes.size() ; i++)
    {
        m_shapes[i]->SetColor(color);
    }
    return const_cast <Group&>(*this);
}

Group* Group::Clone() const
{
    return (new Group(*this));
}
