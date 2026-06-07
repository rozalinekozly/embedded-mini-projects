
#ifndef __ILRD_HRD42_RCSTRING_HPP__
#define __ILRD_HRD42_RCSTRING_HPP__

#include <cstddef> // size_t
#include <iosfwd>  // istream, ostream

namespace hrd42
{
class RCString;
const RCString operator+(const RCString& lhs, const RCString& rhs);

std::istream& operator>>(std::istream& is, RCString& str);
std::ostream& operator<<(std::ostream& os, const RCString& str);

class RCString
{
	class ProxyChar;

public:
	// non-explicit on purpose
	RCString(const char* str = "");
	RCString(const RCString& other);
	RCString& operator=(const RCString& other);
	~RCString();

	bool operator==(const RCString& other) const;
	bool operator!=(const RCString& other) const;
	RCString& operator+=(const RCString& other);

	ProxyChar operator[](size_t idx);
	const char& operator[](size_t idx) const;

	size_t Length() const;
	const char* Cstr() const;

private:
	class SharedData;
	class ProxyChar;
	SharedData* m_data;

	friend std::istream& operator>>(std::istream& is, RCString& str);
	friend std::ostream& operator<<(std::ostream& os, const RCString& str);

	static void DecrementDestroyIfUnique(SharedData* data);
};

class RCString::SharedData
{
	explicit SharedData(const char* str);
	// using generated dtor

	SharedData(const SharedData& other_);
	SharedData& operator=(const SharedData& other);

	bool operator==(const SharedData& other) const;

	size_t GetCounter() const;
	char* GetString();

	SharedData& operator++();
	SharedData& operator--();

	static void* operator new(size_t size, const char* str);
	static void operator delete(void* ptr);

private:
	size_t m_counter;
	char m_str[1];

	friend class RCString;
};

class RCString::ProxyChar
{
public:
	explicit ProxyChar(RCString& rcs, size_t idx);
	ProxyChar& operator=(const ProxyChar& other);

	operator char() const;
	ProxyChar& operator=(char c);

private:
	RCString& m_rcs;
	size_t m_idx;
};

} // namespace hrd42

#endif //__ILRD_HRD42_RCSTRING_HPP__
