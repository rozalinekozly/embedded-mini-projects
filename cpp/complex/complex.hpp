/************************************************************
Description: complex numbers class
Developer: rozaline
Reviewer: shir
Version: 1.6
1 = initial version
1.1 = added operators +=, -=, *=, /= , setters and gettersand test for them
1.2 = added operator == and !=, and test for them
1.3 = added operator +, -, *, / and test for it
1.4 = added operator << and >> and test for it
1.5 = removed infinte loop in operator >> and added assumption on the input format.
1.6: used one variable as placeholder in istream overloading.
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
inline void Complex::SetReal(double real_)
{
    m_real = real_;
}

inline void Complex::SetImg(double img_)
{
    m_img = img_;
}

//operators 
inline bool Complex::operator!=(const Complex& other_) const
{
    return !(*this == other_);
}

inline bool Complex::operator==(const Complex& other_) const
{
    return (m_real == other_.m_real) && (m_img == other_.m_img);
}

//no symmetric requirement operators
inline Complex& Complex::operator+=(const Complex& other_)
{
    m_real += other_.m_real;
    m_img += other_.m_img;
    
    return (*this);
}

inline Complex& Complex::operator-=(const Complex& other_)
{
    m_real -= other_.m_real;
    m_img -= other_.m_img;
    
    return (*this);
}

inline Complex& Complex::operator*=(const Complex& other_)
{
    double real = m_real * other.m_real - m_img * other.m_img;
    double img = m_real * other.m_img + m_img * other_.m_real;
    
    m_real = real;
    m_img = img;
    
    return (*this);
}

inline Complex& Complex::operator/=(const Complex& other_)
{
    double divisor = other_.m_real * other_.m_real + other_.m_img * other_.m_img;
    double real = (m_real * other_.m_real + m_img * other_.m_img) / divisor;
    double img = (m_img * other_.m_real - m_real * other_.m_img) / divisor;
    
    m_real = real;
    m_img = img;
    
    return (*this);   
}  

//friend free func definitions
//a + b
inline const Complex operator+(const Complex& lhs_, const Complex& rhs_)
{
    Complex result(lhs_);
    result += rhs_; //using the operator we defined previously
    return result;
}

//a - b
inline const Complex operator-(const Complex& lhs_, const Complex& rhs_)
{
    Complex result(lhs_);
    result -= rhs_;
    return result;
}

inline const Complex operator*(const Complex& lhs_, const Complex& rhs_)
{
    Complex result(lhs_);
    result *= rhs_;
    return result;
}   

inline const Complex operator/(const Complex& lhs_, const Complex& rhs_)
{
    Complex result(lhs_);
    result /= rhs_;
    return result;
}

//ostream and istream operators overloading
inline std::ostream& operator<<(std::ostream& os, const Complex& complex_)
{
    os << "("<<complex_.m_real << " + " << complex_.m_img << "i" << ")";
    return os;
}

inline std::istream& operator>>(std::istream& is_, Complex& complex_)
{
    char placeholder = 0;

    double real = 0;
    double img = 0;

    //assuming the input format is (real,img i) and valid (ignores whitespaces)
    is_>> placeholder >> real >> placeholder >> img >> placeholder;

    /* */
    complex_.m_real = real;
    complex_.m_img = img;

    return is_;
}
} // namespace hrd42

#endif  // __ILRD_HRD42_COMPLEX_HPP__