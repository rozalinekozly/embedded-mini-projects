
/*
developer: rozaline
reviewer: itay
*/
#include <cstring>  
#include <iomanip>  
#include <iostream>  
#include "rcstring.hpp"

namespace hrd42
{

static const size_t g_max_str_len = 200;

RCString::SharedData::SharedData(const char* str_) : m_counter(1)
{
    strcpy(m_str, str_);
}

void* RCString::SharedData::operator new(size_t size_, const char* str_)
{
    return ::operator new(size_ + strlen(str_)); 
    // size_ already includes the 1 byte of m_str[1],
    // so we only add strlen (the \0 is covered by that 1 byte)
}

void RCString::SharedData::operator delete(void* ptr_)
{
    ::operator delete(ptr_);
}

RCString::SharedData& RCString::SharedData::operator++()
{
    ++m_counter;
    return *this;
}

RCString::SharedData& RCString::SharedData::operator--()
{
    --m_counter;
    return *this;
}

RCString::ProxyChar::ProxyChar(RCString& rcs_, size_t idx_)
    : m_rcs(rcs_), m_idx(idx_)
{}

// read  = conversion operator
RCString::ProxyChar::operator char() const
{
    return m_rcs.Cstr()[m_idx];
}

// write = COW here
RCString::ProxyChar& RCString::ProxyChar::operator=(char c_)
{
    RCString copy(m_rcs.Cstr());       // new independent copy
    copy.m_data->m_str[m_idx] = c_;   // modify the copy
    std::swap(copy.m_data, m_rcs.m_data); // swap into original
    return *this;
}

// s[i] = s[j]
RCString::ProxyChar& RCString::ProxyChar::operator=(const ProxyChar& other_)
{
    return (*this = static_cast<char>(other_));
}



RCString::RCString(const char* str_)
    : m_data(new (str_) SharedData(str_))
{
}

RCString::RCString(const RCString& other_)
    : m_data(other_.m_data)
{
    ++(*m_data);
}

RCString& RCString::operator=(const RCString& other_)
{
    RCString temp(other_);
    std::swap(m_data, temp.m_data);
    return *this;
}

RCString::~RCString()
{
   --(*m_data);
    if (0 == m_data->m_counter)
    {
        delete m_data;
    }
}

bool RCString::operator==(const RCString& other_) const
{
    return (0 == strcmp(Cstr(), other_.Cstr()));
}

bool RCString::operator!=(const RCString& other_) const
{
    return !(*this == other_);
}

RCString& RCString::operator+=(const RCString& other_)
{
    RCString result((*this + other_).Cstr());
    std::swap(m_data, result.m_data);
    return *this;
}

size_t RCString::Length() const
{
    return strlen(Cstr());
}

const char* RCString::Cstr() const
{
    return m_data->m_str;
}

// non-const operator[] returns Proxy
RCString::ProxyChar RCString::operator[](size_t idx_)
{
    return ProxyChar(*this, idx_);
}

// const operator[] direct access, no COW needed
const char& RCString::operator[](size_t idx_) const
{
    return Cstr()[idx_];
}


const RCString operator+(const RCString& lhs_, const RCString& rhs_)
{
    return (RCString(lhs_) += rhs_);
}

std::ostream& operator<<(std::ostream& os_, const RCString& str_)
{
    return (os_ << str_.Cstr());
}

std::istream& operator>>(std::istream& is_, RCString& str_)
{
    char buf[g_max_str_len];
    is_ >> std::setw(g_max_str_len) >> buf;
    str_ = RCString(buf);
    return is_;
}

} // namespace hrd42