#include "shape.hpp"
#include "square.hpp"

using namespace ilrd;
using namespace hrd42;

Square::Square(const ilrd::Point& center,
                    double width ,
                    double angle,
                    COLORS color)  : Shape(center, angle, color)
{
    this->m_width = width;
}

double Square::GetWidth() const
{
    return (this->m_width);
}

Square* Square::Clone() const
{
   
    return (new Square(*this));
}


Square& Square:: Draw() const
{
    int x = (int)(this->GetPoint().GetX());
    int y = (int)(this->GetPoint().GetY());
    int dist = (int)(this->GetWidth()/2); 

    //set vertices x and y
    int x_vertices[2] = {x + dist, x - dist};
    int y_vertices[2] = {y + dist, y - dist};

    Point p1(x_vertices[0], y_vertices[0]);
    Point p2(x_vertices[0], y_vertices[1]);
    Point p3(x_vertices[1], y_vertices[1]);
    Point p4(x_vertices[1], y_vertices[0]);

    Point p[] = {p1, p2, p3, p4};

    for(int i = 0 ; i < 4 ; i++)
    {
        p[i].Revolve(this->GetPoint(), this->GetAngle());
    }

    //set it's angle
    //rotate according to the given angle
    //TODO: add a wrapper on this, its so ugly and repititive
    //polygon drows counter clockwise 
    DrawPolygon(this->GetColor(), 4, 
                (int)p[0].GetX(), (int)p[0].GetY(),
                (int)p[3].GetX(), (int)p[3].GetY(),
                (int)p[2].GetX(), (int)p[2].GetY(),
                (int)p[1].GetX(), (int)p[1].GetY());
    // added in order to support returning a reference from const method
    return const_cast<Square&>(*this);
}
