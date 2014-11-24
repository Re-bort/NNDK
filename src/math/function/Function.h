#ifndef math_Function_h
#define math_Function_h

#include <algorithm> 
#include <math.h>

#include "../vecops.h"

template <class T>
class Function
{
	public:
		Function(){}
        virtual	~Function(){}
		virtual const T operator () (const T& x) const = 0;
        virtual const T operator () (const std::vector<T>& x) const = 0;
        virtual const unsigned dim() const = 0;
};

template <class T>
void transform(const std::matrix<T>& X, std::vector<T>& y, const Function<T>& f)
{
    typename std::matrix<T>::const_iterator x = X.begin();
    typename std::vector<T>::iterator yi = y.begin();
    for(; x != X.end() && yi != y.end(); ++x, ++yi)
        *yi = f(*x);     
}

template <class T>
void transform(const std::vector<T>& x, std::vector<T>& y, const Function<T>& f)
{
    typename std::vector<T>::const_iterator xi = x.begin();
    typename std::vector<T>::iterator yi = y.begin();
    for(; xi != x.end() && yi != y.end(); ++xi, ++yi)
        *yi = f(*xi); 
}

#endif // math_Function_h
