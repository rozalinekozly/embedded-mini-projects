/************************************************************
Description:
Developer:
Reviewer: 
Version: 1
************************************************************/
#ifndef __ILRD_HRD42_COMPLEX_HPP__
#define __ILRD_HRD42_COMPLEX_HPP__

#include <iostream>


namespace hrd42
{

class Complex;
// friend free func declarations
const Complex operator+(const Complex& lhs, const Complex& rhs);
const Complex operator-(const Complex& lhs, const Complex& rhs);
const Complex operator*(const Complex& lhs, const Complex& rhs);
const Complex operator/(const Complex& lhs, const Complex& rhs);

std::istream& operator>>(std::istream& is, Complex& complex);
std::ostream& operator<<(std::ostream& os, const Complex& complex);


//class declaration
class Complex
{
public:
    // non-explicit on purpose
    Complex(double real = 0, double img = 0);
    // rely on generated cctor, copy= and dtor

    double GetReal() const;
    void SetReal(double real);
    double GetImg() const;
    void SetImg(double img);
    
    bool operator!=(const Complex& other) const;
    bool operator==(const Complex& other) const;
    
    Complex& operator+=(const Complex& other);
    Complex& operator-=(const Complex& other);
    Complex& operator*=(const Complex& other);
    Complex& operator/=(const Complex& other);
    

private:
    double m_real;
    double m_img;
    
    //friend declarations
    friend const Complex operator+(const Complex& lhs, const Complex& rhs);
    friend const Complex operator-(const Complex& lhs, const Complex& rhs);
    friend const Complex operator*(const Complex& lhs, const Complex& rhs);
    friend const Complex operator/(const Complex& lhs, const Complex& rhs);
    
    friend std::istream& operator>>(std::istream& is, Complex& complex);
    friend std::ostream& operator<<(std::ostream& os, const Complex& complex);
};
} // namespace hrd42

#endif  // __ILRD_HRD42_COMPLEX_HPP__