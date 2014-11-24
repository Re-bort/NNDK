#ifndef math_function_HillValley_h
#define math_function_HillValley_h

#include <math.h>

#include "Function.h"
#include "../constants.h"

template <class T>
class HillValley
    : public Function<T>
{
    public:
        HillValley(){}
        ~HillValley(){}
        
        inline const T operator () (const T& x) const
        {
            return exp(-(x - 1.0) * (x - 1.0)) - exp(-(x + 1.0) * (x + 1.0));
        }
        
        inline const T operator () (const std::vector<T>& x) const
        {
            return exp(-(pow(x[0] - 1.0, 2.0) + pow(x[1] - 1.0, 2.0)))
                - exp(-(pow(x[0] + 1.0, 2.0) + pow(x[1] + 1.0, 2.0)));
        }
        
        inline const unsigned dim() const
        {
            return 2;
        }
};


#endif //math_function_HillValley_h

