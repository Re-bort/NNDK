#ifndef vectorutil_h
#define vectorutil_h

#include <iostream>
#include <vector>

template <class T>
std::vector<T>& operator << (std::vector<T>& lhs, const std::vector<T>& rhs)
{
	unsigned n = lhs.size();
	lhs.resize(lhs.size() + rhs.size());
	copy(rhs.begin(), rhs.end(), lhs.begin() + n);
	return lhs;
}

template <class T>
std::vector<T>& operator >> (const std::vector<T>& lhs, std::vector<T>& rhs)
{
	unsigned n = rhs.size();
	rhs.resize(rhs.size() + rhs.size());
	copy(lhs.begin(), lhs.end(), rhs.begin() + n);
	return rhs;
}

template <typename T>
std::ostream& operator << (std::ostream& os, const std::vector<T>& v)
{
	typename std::vector<T>::const_iterator vi;
    for(vi = v.begin(); vi < v.end() - 1; ++vi)
    	os << *vi << '\t';
	os << *vi;
	return os;
}



#endif // std::vectorutil_h
