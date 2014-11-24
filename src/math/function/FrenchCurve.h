#ifndef math_func_FrenchCurve_h
#define math_func_FrenchCurve_h

#include "Function.h"

// French Curve---a one-dimensional non-linear regression test function
// Reference: (esp. class default coefficients)
// Wahba G. and S. Wold, "A completely automatic French curve: Fitting
// spline functions by regression by cross-validation", Commun. Stat. Series A,
// Vol. 4, pp.1--17, 1975.

template <class T>
class FrenchCurve
    : public Function<T>
{
	public:
								FrenchCurve()
									: a_(4.26), b_(4.0), c_(3.0)
								{}
								FrenchCurve(const T& a,
										  const T& b,
										  const T& c)
									: a_(a), b_(b), c_(c) {}
	
	 			inline const T	operator () (const T& x) const
								{ 
									return a_*(exp(x) - b_ * exp(-2.0 * x)
										+ c_*exp(-3.0 * x));
								}
                inline const T  operator () (const std::vector<T>& x) const
                                { 
                                    T y = euclidean(x);
                                    return a_ * (exp(y) - b_ * exp(-2.0 * y)
                                        + c_ * exp(-3.0 * y));
                                }                
                                
                inline const unsigned dim() const 
        				        {
            				        return 1;
        				        }
        
	private:
		T 					a_;
		T 					b_;
		T 					c_;
};

#endif  // math_func_FrenchCurve_h

