#include <iostream>
#include "complex.hpp"

using namespace hrd42;
using namespace std;
int main()
{
    Complex c1(1, 2); //1 + 2i
    Complex c2(3, 4);  // 3 + 4i
    
    cout << "testing ctor" << endl;
    cout << "c1: " << c1 << endl;
    cout << "c2: " << c2 << endl;
    
    std::cout << "testing operator +=" << std::endl;
    c1 += c2; // 4 + 6i
    cout << "c1 += c2 =  " << c1 << endl;
    
    std::cout << "testing operator -=" << std::endl;
    c1 -= c2; // 1 + 2i
    cout << "c1 -= c2 =  " << c1 << endl;   

    std::cout << "testing operator *=" << std::endl;
    c1 *= c2; // (1*3 - 2*4) + (1*4 + 2*3)i = -5 + 10i
    cout << "c1 *= c2 =  " << c1 << endl;

    std::cout << "testing operator /=" << std::endl;
    c1 /= c2; // (-5*3 + 10*4) + (-5*4 - 10*3)i / (3^2 + 4^2) = 1 + 2i
    cout << "c1 /= c2 =  " << c1 << endl;
    
    return 0;
}
