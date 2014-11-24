#ifndef math_function_Friedman_h
#define math_function_Friedman_h

#include <math.h>

#include "Function.h"
#include "../constants.h"

//	Reference: 
//  Jerome Friedman in "Multivariate Adaptive Regression Splines", 
//	Technical Report No. 102, November 1988, Laboratory for Computational
//  Statistics, Department of Statistics, Stanford University.
//
//  Available from www.cs.toronto.edu/~delve/ (last visited on 5/6/2006)

template <class T>
class Friedman
	: public Function<T>
{
	public:
		Friedman(){}
        ~Friedman(){}
        
        inline const T operator () (const T& x) const
        {
            return 10.0*sin(pi()*x);
        }
        
        inline const T operator () (const std::vector<T>& x) const
		{
			return 10.0*sin(pi()*x[0]*x[1]) + 20.0*pow(x[2]-0.5, 2.0) 
				+ 10.0*x[3] + 5.0*x[4]; 
		}
        
        inline const unsigned dim() const
        {
            return 5;
        }
};


#endif //math_function_Friedman_h
