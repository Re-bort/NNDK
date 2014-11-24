#ifndef Sine_h
#define Sine_h

#include <math.h>

#include "Function.h"
#include "../vecops.h"

template <class T>
class Sine
    : public Function<T>
{
	public:
        inline const T operator () (const T& x) const 
            { return sin(x); }
        inline const T operator () (const std::vector<T>& x) const
            { return sin(sqrt(x * x)); }
        inline const unsigned dim() const
        {
            return 2;
        }
};	

#endif // Sine_h
