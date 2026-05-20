/*------------------------------------------------------------
Description: implementation of String class
Developer: rozaline
Reviewer:
Version: 1
more version info: 
 -------------------------------------------------------------*/
#include <iostream>
#include <cstddef>
#include <string.h>

#include "string.hpp"

enum 
{
    MAX_LEN_STR = 200
};
 namespace hrd42
{

/* ----- special public member funcitons ------ */ 
//ctor (by default it initailize with empty string)
String::String(const char* str)
{
    this->m_str = new char[strlen (str) + 1];
    strcpy(m_str, str);
}

//dtor 
String::~String()
{
    delete[] this->m_str;
    this->m_str = nullptr;
}

//cctor
String::String(const String& other)
{
    this->m_str = new char[strlen(other.m_str) + 1];
    strcpy(this->m_str, other.m_str);
}

//copy assignment operator
String& String:: operator=(const String& other)
{
    strcpy(this->m_str, other.m_str);
    return (*this);
}
// -----------------------------------------------------
bool String::operator==(const String& other) const
{
    if(strcmp(this->m_str , other.m_str) != 0)
    {
        return false;
    }
    return true;
}

bool String::operator!=(const String& other) const
{
    return !(this->operator==(other));
}

// --------------------------------------------------
size_t String::Length() const
{
    return(strlen(this->m_str));
}

String& String::operator+=(const String& other_)
{
    char* concated_str = new char[this->Length() + other_.Length() + 1];
    strcpy(concated_str, this->m_str);
    strcat(concated_str, other_.m_str);

    delete[] (this->m_str);
    this->m_str = concated_str;

    return (*this);

}

const char* String::Cstr() const
{
    return (this->m_str);
}

const String operator+(const String& lhs, const String& rhs)
{
    return (String(lhs)+= rhs);
}

std::istream& operator>>(std::istream& is, String& str)
{
    char str_holder[MAX_LEN_STR] = {'\0'};

    is.getline(str_holder, MAX_LEN_STR);
    //assuming input is valid
    str = String(str_holder);
    return is;
}

std::ostream& operator<<(std::ostream& os, const String& str)
{
    return os << str.m_str;
}

};//end of namespace hrd42
