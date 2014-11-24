#ifndef math_func_Nonlinear_h
#define math_func_Nonlinear_h

#include "Function.h"

template <class T>
class Nonlinear
    : public Function<T>
{
    public:
                                Nonlinear()
                                    : a_(1.0), b_(0.1), c_(0.1)
                                {}
                                Nonlinear(const T& a,
                                          const T& b,
                                          const T& c)
                                    : a_(a), b_(b), c_(c) {}
    
        inline const T      operator () (const T& x) const
                                { 
                                    return a_*x*x*x + b_*cos(x) + c_*sin(x);
                                }
        inline const T     operator () (const std::vector<T>& x) const
        {
            return a_*pow(x[0], 3.0) + b_ * cos(x[1]) + c_*sin(x[2]);
        }
        inline const unsigned dim() const
        {
            return 3;
        }
        
    private:
        T                   a_;
        T                   b_;
        T                   c_;
};

#endif //math_func_Nonlinear_h
