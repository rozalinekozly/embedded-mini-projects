#include <cstring> 
#include <iostream>

#include "rcstring.hpp"

using namespace hrd42;

const size_t g_max_len = 200;

class RCString::SharedData 
{

  public:

    //cctor and operator= are blocked , this class take no 
    //responsibility neither decisions
    explicit SharedData(const char* str);
    
    size_t GetCounter();
    char* GetString();

    SharedData& operator++();
    SharedData& operator--();
        
    void* operator new(size_t size_, const char* str_);
    void operator delete(void* ptr_);

  private:
    size_t m_count;
    char m_data[1];

    //blocked (moved responsibility to RCString class)
    SharedData& operator=(const SharedData& other_);
    SharedData(const SharedData& other_);

};
//----------------------------------------------------------------
//SHARED DATA IMPLEMENTATION
RCString::SharedData::SharedData(const char* str) : m_count(1)
{
    strcpy(m_data, str);
}

RCString::SharedData& RCString::SharedData::operator++()
{
    ++m_count;
    return *this;
}

RCString::SharedData& RCString::SharedData::operator--()
{
    --m_count;
    return *this;
}

void* RCString::SharedData::operator new(size_t size_, const char* str_)
{
    return ::operator new(size_ + strlen(str_));
}

void RCString::SharedData::operator delete(void* ptr_)
{
    ::operator delete(ptr_);
}

//----------------------------------------------------------------
//RC String IMPLEMENTATION
RCString::RCString(const char* str) : m_shared_data(new (str) SharedData(str))
{}

RCString::RCString(const RCString& other) : m_shared_data(other.m_shared_data)
{
    ++(*m_shared_data);
}

RCString& RCString::operator=(const RCString& other)
{
    // self assignment check
    if (this != &other)  
    {
        --m_shared_data;
        //if its not shared anymore, destroy it
        if (0 == m_shared_data->GetCounter())
        {
            delete m_shared_data;
        }
        //set the new data to point at the other data and increase the counter
        m_shared_data = other.m_shared_data;
        ++m_shared_data;
    }
    return *this;
}

RCString::~RCString()
{
    --(*m_shared_data);
    //if its not shared anymore, destroy it
    if (0 == m_shared_data->GetCounter())
    {
        delete m_shared_data;
        //handle dangling pointer
        m_shared_data = 0;
    }
}

bool RCString::operator==(const RCString& other) const
{
    return (0 == strcmp(m_shared_data->GetString(), other.m_shared_data->GetString()));
}

bool RCString::operator!=(const RCString& other) const
{
    return !(*this == other);
}

RCString& RCString::operator+=(const RCString& other)
{
    // to protect from a situation such str += str;
    char* temp = new char[this->Length() + other.Length() + 1]; //allocate properly sized buffer
    strcpy(temp, this->Cstr()); 
    strcat(temp, other.Cstr());

    //decrease the counter of the current data, if its not shared anymore, destroy it
    --(*m_shared_data);
    if (0 == m_shared_data->GetCounter())
    {
        delete m_shared_data;
        m_shared_data = nullptr;
    }
    //set the new data to point at the new string and increase the counter
    SharedData* new_data = new (temp) SharedData(temp);
    m_shared_data = new_data;
    delete[] temp;

    return *this;
}

const char& RCString::operator[](size_t idx) const
{
    return Cstr()[idx];
}

size_t RCString::Length() const
{
    return strlen(Cstr());

}

const char* RCString::Cstr() const
{
    return m_shared_data->GetString();
}

std::istream& operator>>(std::istream& is, RCString& str)
{
    char read_buf[g_max_len];
    is.getline(read_buf, g_max_len); 
    str = RCString(read_buf);
    return is;
}

std::ostream& operator<<(std::ostream& os, const RCString& str)
{
    os << str.Cstr();
    return os;
}

const RCString operator+(const RCString& lhs, const RCString& rhs)
{
    return (RCString(lhs) += rhs);
}



