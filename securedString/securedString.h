#ifndef SECUREDSTRING_H
#define SECUREDSTRING_H

#include <Windows.h>

template <typename T>
class SecuredString
{
public:
	template<typename T2>
	friend void swap(SecuredString<T2> &, SecuredString<T2> &);

private:
	T *data;
	unsigned int datacnt;
	//bool owningData;
	bool comparememory(void *lhs, void *rhs, unsigned int length) const;

public:
	SecuredString();
	SecuredString(T *data);
	SecuredString(const SecuredString &);
	SecuredString(SecuredString &&);

	void gainControl(T *data);

	~SecuredString();
	bool operator==(const SecuredString &other) const;
	bool operator!=(const SecuredString &other) const;
	SecuredString &operator=(SecuredString);
	SecuredString &operator+=(const SecuredString &rhs);
	const SecuredString operator+(const SecuredString &other) const;

	T *c_str() const;
	T *release();

	//for testing use
	unsigned int __get_length();
};

template <typename T>
bool SecuredString<T>::comparememory(void *lhs, void *rhs, unsigned int length) const
{
	for (unsigned int counter = 0; counter < length; counter++)
	{
		if (*static_cast<unsigned char *>(lhs) != *static_cast<unsigned char *>(rhs))
			return false;
	}
	return true;
}

template <typename T>
void swap(SecuredString<T> &lhs, SecuredString<T> &rhs)
{
	T *tmp_data = lhs.data;
	lhs.data = rhs.data;
	rhs.data = tmp_data;

	unsigned int tmp_datacnt = lhs.datacnt;
	lhs.datacnt = rhs.datacnt;
	rhs.datacnt = tmp_datacnt;
}

template <typename T>
const SecuredString<T> SecuredString<T>::operator+(const SecuredString<T> &other) const {
	SecuredString<T> result = *this;
	result += other;
	return result;
}

template <typename T>
bool SecuredString<T>::operator == (const SecuredString<T> &other) const
{
	if (datacnt != other.datacnt)
		return false;
	return comparememory(reinterpret_cast<void *>(data), reinterpret_cast<void *>(other.data), datacnt*sizeof(T));
}

template <typename T>
bool SecuredString<T>::operator != (const SecuredString<T> &other) const
{
	return !this->operator==(other);
}

template<typename T>
SecuredString<T> &SecuredString<T>::operator+=(const SecuredString<T> &rhs)
{
	SecuredString<T> tmp;
	//create data
	tmp.datacnt = datacnt + rhs.datacnt - 1;
	tmp.data = new T[datacnt + rhs.datacnt - 1]; //only 1 padding \0
	memcpy(tmp.data, data, (datacnt-1)*sizeof(T));
	memcpy(tmp.data + (datacnt - 1), rhs.data, rhs.datacnt*sizeof(T));
	
	swap(*this, tmp);

	return *this;
}

template<typename T>
void SecuredString<T>::gainControl(T *data)
{
	SecuredString<T> tmp{};
	swap<T>(*this, tmp);

	this->data = data;
	
	//find data count
	while (*data != 0x0)
	{
		this->datacnt++;
		data++;
	}
}

template <typename T>
SecuredString<T>::SecuredString()
	: data{ nullptr }, datacnt{ 0 }
{

}

//copy and swap idiom, together with combined operator=(const &) and operator=(&&)
template <typename T>
SecuredString<T> &SecuredString<T>::operator=(SecuredString rhs)
{
	swap(*this, rhs);
	return *this;
}

template <typename T>
SecuredString<T>::SecuredString(T *data)
	:datacnt{ 1 } //cuz there is always at least a null character
{
	//find data count
	T *tmp = data;
	while (*tmp != 0x0)
	{
		datacnt++;
		tmp++;
	}

	//create data
	this->data = new T[datacnt];
	memcpy(this->data, data, datacnt*sizeof(T));
}

template <typename T>
SecuredString<T>::SecuredString(const SecuredString &rhs)
	:data{ nullptr }, datacnt{rhs.datacnt}
{
	if (datacnt != 0)
	{
		//create data
		data = new T[datacnt];
		memcpy(data, rhs.data, datacnt*sizeof(T));
	}
}

template <typename T>
SecuredString<T>::SecuredString(SecuredString &&rhs)
	:SecuredString{ rhs }
{
	rhs.data = nullptr;
	rhs.datacnt = 0;
}

template <typename T>
SecuredString<T>::~SecuredString()
{
	if (datacnt != 0)
	{
		SecureZeroMemory(data, datacnt*sizeof(T));
		delete[] data;
	}
}

template <typename T>
T *SecuredString<T>::c_str() const
{
	return data;
}

template <typename T>
T *SecuredString<T>::release()
{
	T *retval = data;
	datacnt = 0;
	data = nullptr;
	return retval;
}

template < typename T >
unsigned int SecuredString<T>::__get_length()
{
	return datacnt;
}

#endif