/*------------------------------------------------------------
Description: implementation of String class
Developer: rozaline
Reviewer:
Version: 1
more version info: 
 -------------------------------------------------------------*/
#include <iosfwd>
#include <cstddef>
#include <string.h>

#include "string.hpp"

 namespace hrd42
{

/* ----- special public member funcitons ------ */ 
//ctor (by default it initailize with empty string)
String::String(const char* str)
{
    m_str = new char[strlen (str + 1)];
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
    this->m_str = other.m_str;
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
/*String& String::operator+=(const String& other_)
{

}*/
};//end of namespace hrd42
