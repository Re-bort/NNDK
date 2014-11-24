#ifndef glr_Regression_h
#define glr_Regression_h

#include "../math/constants.h"
#include "../math/matops.h"

#define VECTOR std::vector<double>
#define MATRIX std::matrix<double>

namespace glr {
	
class Regression
{
	public:
						        Regression(const MATRIX& X, const VECTOR& Y);
										   
		const VECTOR&	        Y() const;
		const double			operator() (const VECTOR& x) const;
		const VECTOR	        operator() (const MATRIX& X) const;
	    void                    setY(const VECTOR& Y);
        
	protected:
		const MATRIX *			X_;
		const VECTOR *        	Y_;
		VECTOR					B_;
		MATRIX					S_;
		MATRIX					Si_;        
                                Regression(){} // Empty constructor hidden
};

class Regression1
{
    public:
                                Regression1(const VECTOR& X, const VECTOR& Y);
                                           
        const VECTOR&           Y() const;
        const double            operator() (const double& x) const;
        const VECTOR            operator() (const VECTOR& X) const;
        void                    setY(const VECTOR& Y);
    
    protected:
        const VECTOR * 			X_;
        const VECTOR *          Y_;
        double					b_;
                                Regression1(){} // Empty constructor hidden
};

} // namespace glr

#undef VECTOR
#undef MATRIX
#endif // glr_Regression_h
