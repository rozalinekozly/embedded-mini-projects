#ifndef __ILRD_HRD42_STRING_HPP__
#define __ILRD_HRD42_STRING_HPP__

#include <iosfwd> // istream, ostream
#include <cstddef>  // size_t

namespace hrd42
{

const String operator+(const String& lhs, const String& rhs);

std::istream& operator>>(std::istream& is, String& str);
std::ostream& operator<<(std::ostream& os, const String& str);

class String
{
public:
    // non-explicit on purpose
	String(const char* str = "");
	String(const String& other);
	String& operator=(const String& other);
	~String();

	bool operator==(const String& other) const;
	bool operator!=(const String& other) const;
	String& operator+=(const String& other);
	
	char& operator[](size_t idx);
    const char& operator[](size_t idx) const;
    
	size_t Length() const;
	const char* Cstr() const;

private:
	char* m_str;

	friend std::istream& operator>>(std::istream& is, String& str);
	friend std::ostream& operator<<(std::ostream& os, const String& str);
};

}; // namespace hrd42

#endif //__ILRD_HRD42_STRING_HPP__