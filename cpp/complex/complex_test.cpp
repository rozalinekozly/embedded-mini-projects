#include <iostream>
#include "complex.hpp"

using namespace hrd42;
using namespace std;

void TestPrinting();
void TestSymmetricOps();
void TestCtors();
void TestNonSymmetricOPs();
void TestSettersGetters();
void TestInput();

int main()
{

    TestPrinting();

    TestCtors();

    TestSettersGetters();

    TestNonSymmetricOPs();
    
    TestSymmetricOps();

    TestInput();

    return 0;
}


void TestCtors()
{
    cout << "testing ctor given real and img arguments:" << endl;
    Complex c1(1, 2); //1 + 2i
    cout << "c1: " << c1 << endl<<endl;

    cout << "testing ctor given no arguments:" << endl;
    Complex c2;  // 0 + 0i
    cout << "c2: " << c2 << endl<<endl;

    cout << "testing ctor given only real argument:" << endl;
    Complex c3(3); // 3 + 0i
    cout << "c3: " << c3 << endl<<endl;
}

void TestSettersGetters()
{
    Complex c1(1, 2);   

    cout << "testing setters" << endl;
    c1.SetReal(5);
    c1.SetImg(6);
    cout << "c1 after setting real to 5 and img to 6: " << c1 << endl;

    cout << "testing getters" << endl;  
    cout << "c1 real part: " << c1.GetReal() << endl;
    cout << "c1 img part: " << c1.GetImg() << endl;
}

void TestNonSymmetricOPs()
{
    std::cout << "testing operator +=" << std::endl;
    Complex c1(1, 2);
    Complex c2(3, 4);
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

    cout << "testing operator ==" << endl;
    cout << "c1 " << c1 << " == c2 " << c2 << ": " << (c1 == c2) << endl;
    
    cout << "testing operator !=" << endl;
    cout << "c1 " << c1 << " != c2 " << c2 << ": " << (c1 != c2) << endl;
}

void TestPrinting()
{
    cout << "testing operator <<" << endl;
    Complex c1(1, 2);
    cout << "c1: " << c1 << endl;
}
void TestSymmetricOps()
{
    Complex c1(5, 6);
    Complex c2(3, 4);

    cout << "testing operator +" << endl;
    Complex c3 = c1 + c2; 
    cout << "c1 + c2 = " << c1 << " + " << c2 << " = " << c3 << endl <<endl;

    cout << "testing operator -" << endl;
    Complex c4 = c1 - c2; 
    cout << "c1 - c2 = " << c1 << " - " << c2 << " = " << c4 << endl << endl;   

    cout << "testing operator *" << endl;
    Complex c5 = c1 * c2; 
    cout << "c1 * c2 = " << c1 << " * " << c2 << " = " << c5 << endl << endl;

    cout << "testing operator /" << endl;
    Complex c6 = c1 / c2; 
    cout << "c1 / c2 = " << c1 << " / " << c2 << " = " << c6 << endl << endl;
}

void TestInput()
{
    cout << "testing operator >>" << endl;
    Complex c1;
    cout << "enter a complex number in the format (real,img i): ";
    cin >> c1;
    cout << "you entered: " << c1 << endl;
}