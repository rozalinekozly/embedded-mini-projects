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
    m_str = const_cast<char*> (str);
}

//dtor 
String::~String()
{
    //do nothing
}

//cctor
String::String(const String& other)
{
    this->m_str = other.m_str;
}

//copy assignment operator
String& String:: operator=(const String& other)
{
    this->m_str = other.m_str;
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
};//end of namespace hrd42
