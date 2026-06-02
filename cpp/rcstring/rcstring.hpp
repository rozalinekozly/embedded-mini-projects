#ifndef __ILRD_HRD42_STRING_HPP__
#define __ILRD_HRD42_STRING_HPP__

#include <iosfwd> // istream, ostream
#include <cstddef>  // size_t

namespace hrd42
{
class RCString;

const RCString operator+(const RCString& lhs, const RCString& rhs);

std::istream& operator>>(std::istream& is, RCString& str);
std::ostream& operator<<(std::ostream& os, const RCString& str);

class RCString
{
  public:
    class SharedData;
   
    // non-explicit on purpose
    RCString(const char* str = "");
    RCString(const RCString& other);
    RCString& operator=(const RCString& other);
    ~RCString();

    bool operator==(const RCString& other) const;
    bool operator!=(const RCString& other) const;
    RCString& operator+=(const RCString& other);

    const char& operator[](size_t idx) const;

    size_t Length() const;
    const char* Cstr() const;

  private:
    SharedData* m_shared_data;

    friend std::istream& operator>>(std::istream& is, RCString& str);
    friend std::ostream& operator<<(std::ostream& os, const RCString& str);
};

} // namespace hrd42

#endif //__ILRD_HRD42_RCSTRING_HPP__