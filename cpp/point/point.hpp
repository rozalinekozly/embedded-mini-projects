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
    double Length();
    void Print();
    
    //data
    int m_x;
    int m_y;
};

}//namespace hrd42
#endif  // __ILRD_HRD42_COMPLEX_HPP__