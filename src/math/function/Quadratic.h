#ifndef math_func_quadratic_h
#define math_func_quadratic_h

#include "Function.h"

template <class T>
class Quadratic
    : public Function<T>
{
	public:
								Quadratic()
									: a_(1.0), b_(0.0), c_(0.0)
								{}
								Quadratic(const T& a,
										  const T& b,
										  const T& c)
									: a_(a), b_(b), c_(c) {}
	
		inline const T		operator () (const T& x) const
								{ 
									return a_*x*x + b_*x + c_;
								}
        inline const T     operator () (const std::vector<T>& x) const
        {
            return a_* (x * x) + b_ * sqrt(x * x) + c_;
        }
        inline const unsigned dim() const 
        {
            return 2;
        }
        
	private:
		T 					a_;
		T 					b_;
		T 					c_;
};

#endif //math_func_quadratic_h
