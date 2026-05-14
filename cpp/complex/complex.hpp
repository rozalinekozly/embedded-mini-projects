/************************************************************
Description: complex numbers class
Developer: rozaline
Reviewer: shir
Version: 1.5
1 = initial version
1.1 = added operators +=, -=, *=, /= , setters and gettersand test for them
1.2 = added operator == and !=, and test for them
1.3 = added operator +, -, *, / and test for it
1.4 = added operator << and >> and test for it
1.5 = removed infinte loop in operator >> and added assumption on the input format.
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
//ctor
inline Complex::Complex(double real, double img) : m_real(real), m_img(img){}

//getters
inline double Complex::GetReal() const
{
    return m_real;
}

inline double Complex::GetImg() const
{
    return m_img;
}

//setters
inline void Complex::SetReal(double real)
{
    m_real = real;
}

inline void Complex::SetImg(double img)
{
    m_img = img;
}

//operators 
inline bool Complex::operator!=(const Complex& other) const
{
    return !(*this == other);
}

inline bool Complex::operator==(const Complex& other) const
{
    return (m_real == other.m_real) && (m_img == other.m_img);
}

//no symmetric requirement operators
inline Complex& Complex::operator+=(const Complex& other)
{
    m_real += other.m_real;
    m_img += other.m_img;
    
    return (*this);
}

inline Complex& Complex::operator-=(const Complex& other)
{
    m_real -= other.m_real;
    m_img -= other.m_img;
    
    return (*this);
}

inline Complex& Complex::operator*=(const Complex& other)
{
    double real = m_real * other.m_real - m_img * other.m_img;
    double img = m_real * other.m_img + m_img * other.m_real;
    
    m_real = real;
    m_img = img;
    
    return (*this);
}

inline Complex& Complex::operator/=(const Complex& other)
{
    double divisor = other.m_real * other.m_real + other.m_img * other.m_img;
    double real = (m_real * other.m_real + m_img * other.m_img) / divisor;
    double img = (m_img * other.m_real - m_real * other.m_img) / divisor;
    
    m_real = real;
    m_img = img;
    
    return (*this);   
}  

//friend free func definitions
//a + b
inline const Complex operator+(const Complex& lhs, const Complex& rhs)
{
    Complex result(lhs);
    result += rhs; //using the operator we defined previously
    return result;
}

//a - b
inline const Complex operator-(const Complex& lhs, const Complex& rhs)
{
    Complex result(lhs);
    result -= rhs;
    return result;
}

inline const Complex operator*(const Complex& lhs, const Complex& rhs)
{
    Complex result(lhs);
    result *= rhs;
    return result;
}   

inline const Complex operator/(const Complex& lhs, const Complex& rhs)
{
    Complex result(lhs);
    result /= rhs;
    return result;
}

//ostream and istream operators overloading
inline std::ostream& operator<<(std::ostream& os, const Complex& complex)
{
    os << "("<<complex.m_real << " + " << complex.m_img << "i" << ")";
    return os;
}

inline std::istream& operator>>(std::istream& is, Complex& complex)
{
    char open_paren = 0;
    char comma = 0;
    char close_paren = 0;

    double real = 0;
    double img = 0;

    //assuming the input format is (real,img i) and valid (ignores whitespaces)
    is >> open_paren >> real >> comma >> img >> close_paren;

    complex.m_real = real;
    complex.m_img = img;

    return is;
}
} // namespace hrd42

#endif  // __ILRD_HRD42_COMPLEX_HPP__