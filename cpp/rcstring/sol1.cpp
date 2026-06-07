
#include "rcstring.hpp" //API

#include <cstdio>   // BUFSIZ
#include <cstring>  // strlen
#include <iomanip>  // std::setw
#include <iostream> // iostream

namespace hrd42
{
const size_t g_max_str_len = BUFSIZ;

/********************** Shared Data *************************/
RCString::SharedData::SharedData(const char* str) : m_counter(1)
{
	strcpy(m_str, str);
}

RCString::SharedData::SharedData(const SharedData& other_) : m_counter(1)
{
	memmove(m_str, other_.m_str, strlen(m_str) + 1);
}

RCString::SharedData& RCString::SharedData::operator=(const SharedData& other_)
{
	m_counter = 1;
	memmove(m_str, other_.m_str, strlen(m_str) + 1);
	return *this;
}

char* RCString::SharedData::GetString()
{
	return m_str;
}

size_t RCString::SharedData::GetCounter()
{
	return m_counter;
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

void* RCString::SharedData::operator new(size_t size_, const char* str_)
{
	return ::operator new(size_ + strlen(str_));
}

void RCString::SharedData::operator delete(void* ptr_)
{
	::operator delete(ptr_);
}

/********************** Char Wrapper *************************/
RCString::CharWrapper::CharWrapper(RCString& rcs_, size_t idx_)
    : m_rcs(rcs_), m_idx(idx_)
{
}

RCString::CharWrapper::operator char() const
{
	return m_rcs.Cstr()[m_idx];
}

RCString::CharWrapper& RCString::CharWrapper::operator=(char c_)
{
	RCString copy(m_rcs.Cstr());
	copy.m_data->m_str[m_idx] = c_;

	std::swap(copy, m_rcs);

	return *this;
}

RCString::CharWrapper&
RCString::CharWrapper::operator=(const RCString::CharWrapper& other_)
{
	return (*this = static_cast<char>(other_));
}

/************************ RCString ****************************/
RCString::RCString(const char* str_) : m_data((new (str_) SharedData(str_)))
{
}

RCString::RCString(const RCString& other_) : m_data(other_.m_data)
{
	++(*other_.m_data);
}

RCString& RCString::operator=(const RCString& other_)
{
	RCString temp(other_);
	std::swap(m_data, temp.m_data);

	return *this;
}

RCString::~RCString()
{
	DecrementDestroyIfUnique(m_data);
}

bool RCString::operator==(const RCString& other_) const
{
	return (0 == strcmp(m_data->GetString(), other_.m_data->GetString()));
}

bool RCString::operator!=(const RCString& other_) const
{
	return (!(*this == other_));
}

RCString& RCString::operator+=(const RCString& other_)
{
	char* temp = new char[Length() + other_.Length() + 1];
	strcpy(temp, Cstr());
	strcat(temp, other_.Cstr());

	DecrementDestroyIfUnique(m_data);

	m_data = new (temp) SharedData(temp);

	delete[] temp;
	return *this;
}

RCString::CharWrapper RCString::operator[](size_t idx_)
{
	return RCString::CharWrapper(*this, idx_);
}

const char& RCString::operator[](size_t idx_) const
{
	return Cstr()[idx_];
}

size_t RCString::Length() const
{
	return strlen(Cstr());
}

const char* RCString::Cstr() const
{
	return m_data->GetString();
}

// friends
std::istream& operator>>(std::istream& is_, RCString& str_)
{
	char buf[g_max_str_len];
	is_ >> std::setw(g_max_str_len) >> buf;

	str_ = RCString(buf);

	return is_;
}

std::ostream& operator<<(std::ostream& os_, const RCString& str_)
{
	os_ << str_.Cstr();

	return os_;
}

// free func
const RCString operator+(const RCString& lhs_, const RCString& rhs_)
{
	return (RCString(lhs_) += rhs_);
}

// helpers
void RCString::DecrementDestroyIfUnique(SharedData* data_)
{
	--(*data_);
	if (0 == data_->GetCounter())
	{
		delete (data_);

		data_ = 0;
	}
}

} // namespace hrd42
