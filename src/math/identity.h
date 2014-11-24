#ifndef math_identity_h
#define math_identity_h

#include "../stl/matrix.h"

namespace std {

template <class T>
class identity
	: public std::matrix<T>
{
	public:
								identity();
								identity(unsigned n);
};

template <class T>
identity<T>::identity()
	: std::matrix<T>(1, 1, 1)
{}

template <class T>
identity<T>::identity(unsigned n)
	: std::matrix<T>(n, n, 0)
{
	for(unsigned i = 0; i < n; ++i) 
		(*this)[i][i] = 1;
}

} // namespace std

#endif //math_identity_h
