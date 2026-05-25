#include <iostream>

#include "point.hpp"

using namespace std;
using namespace hrd42;

void TestPrint();
void TestCtor();
void TestLength();
//---------------
void TestAdd();

int main()
{
    TestPrint();
    TestCtor();
    TestLength();
    TestAdd();

    return 0;
}
/*--------------------------------------------------------- */
void TestCtor()
{
    cout<<"testing ctor, handling argument call, no arguments at all and partial arguments" <<endl;
    Point p1;
    cout<<p1.m_x<<","<<p1.m_y<<endl;
    Point p2(5);
    cout<<p2.m_x<<","<<p2.m_y<<endl;
    Point p3(9, 4);
    cout<<p3.m_x<<","<<p3.m_y<<endl;
}
void TestPrint()
{
    Point p1(5, 8);
    p1.Print('(','}');
}
void TestLength()
{
    Point p1(1,1);
    float len = p1.Length();
    
    p1.Print();
    cout<<"length from 0,0 is "<< len << endl;
}

void TestAdd()
{
    Point p1(3,8);
    Point p2(5,12);

    cout<<"sum of ";
    p1.Print();
    cout<<"and ";
    p2.Print();
    cout<<"is ";
    Add(p1,p2).Print();

}