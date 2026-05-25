#ifndef __ILRD_HRD42_POINT_HPP__
#define __ILRD_HRD42_POINT_HPP__

namespace hrd42
{

struct Point
{
    //methods
    Point(int x = 0, int y = 0);
    //relying on generated dtor, cctor and copy assignment operator
    void AdjustBy(Point p);
    float Length();
    void Print(char first_parantheses = '(', char second_parantheses = ')');

    //data
    int m_x;
    int m_y;
};

//non-members
Point Add(Point p1, Point p2);

}//namespace hrd42
#endif  // __ILRD_HRD42_COMPLEX_HPP__