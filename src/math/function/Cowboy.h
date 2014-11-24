#ifndef math_function_Cowboy_h
#define math_function_Cowboy_h

#include <math.h>

#include "Function.h"
#include "../vecops.h"

template <class T>
class Cowboy
    : public Function<T>
{
        
	public:
        Cowboy(){}
        ~Cowboy(){}
		inline const T operator () (const T& x) const
        {
            return sin(sqrt(x));
        }
        
        inline const T operator () (const std::vector<T>& x) const 
		{ 
			return sin(sqrt(x[0] * x[0] + x[1] * x[1]));
		}
        
        inline const unsigned dim() const
        { 
            return 2; 
        } 
};

#endif //math_function_Cowboy_h
