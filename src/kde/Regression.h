#ifndef kde_Regression_h
#define kde_Regression_h

#include "Estimator.h"
#include "../math/constants.h"

#define VECTOR std::vector<double>
#define MATRIX std::matrix<double>

namespace kde {
	
template <class KERNEL>
class Regression
    : public Estimator<KERNEL>
{
	public:
						        Regression(const MATRIX& X, const VECTOR& Y);
										   
		const VECTOR&	        Y() const;
		const double			operator() (const VECTOR& x) const;
		const VECTOR	        operator() (const MATRIX& X) const;
	    void                    setY(const VECTOR& Y);
        
        const VECTOR            leaveOneOut() const;
        
	protected:
		const VECTOR *	        Y_;
        
                                Regression(){} // Empty constructor hidden
};

template <class KERNEL>
Regression<KERNEL>::Regression(const MATRIX& X, const VECTOR& Y)
	: Estimator<KERNEL>(X), Y_(&Y)
{}

template <class KERNEL>	
const VECTOR& Regression<KERNEL>::Y() const
{
	return * Y_;
}

template <class KERNEL>   
void Regression<KERNEL>::setY(const VECTOR& Y)
{
    Y_ = &Y;
}

template <class KERNEL>
const double Regression<KERNEL>::operator () (const VECTOR& x) const
												   
{
	MATRIX::const_iterator xj = this->X_->begin();
	VECTOR::const_iterator yj = this->Y_->begin();
	double m = 0.0;
	double den = 0.0;
	for(; xj != this->X_->end() && yj != this->Y_->end(); ++yj, ++xj)
	{
		double k = this->K(x - *xj);
		m += (*yj)*k;
		den += k;
	}
	if(den < small())
		den = small();
	m /= den;
	return m;
}

template <class KERNEL>
const VECTOR Regression<KERNEL>::operator () (const MATRIX& X) const
{
	VECTOR M(X.rows());
    VECTOR::iterator m = M.begin();
    MATRIX::const_iterator x = X.begin();
    for(; x != X.end(); ++x, ++m)
       *m = (*this)(*x);
    return M;
}

template <class KERNEL>
const VECTOR Regression<KERNEL>::leaveOneOut() const
{
    VECTOR M(this->X_->rows());
    VECTOR::iterator m = M.begin();
    MATRIX::const_iterator x = this->X_->begin();
    for(; x != this->X_->end(); ++x, ++m)
    {
        *m = 0.0;
        double den = 0.0;
    
        MATRIX::const_iterator xj = this->X_->begin();
        VECTOR::const_iterator yj = this->Y_->begin();
        for(; xj != x && yj != Y_->end(); ++xj, ++yj)
        {
            double k = this->K(*x - *xj);
            *m += (*yj)*k;
            den += k;
        }
        ++xj;   // Leave out case of *xj = x
        ++yj;
        for(; xj != this->X_->end() && yj != this->Y_->end(); ++xj, ++yj)
        {
            double k = this->K(*x - *xj);
            *m += (*yj)*k;
            den += k;
        }
        if(den < small())
            den = small();
        *m /= den;
    }
    return M;
}
        
    



template <class KERNEL1D>
class Regression1D
    : public Estimator1D<KERNEL1D>
{
    public:
                                Regression1D(const VECTOR& X, const VECTOR& Y);
                                           
        const VECTOR&           Y() const;
        const double            operator() (const double& x) const;
        const VECTOR            operator() (const VECTOR& X) const;
        void                    setY(const VECTOR& Y);
    
    protected:
        const VECTOR *          Y_;
        
                                Regression1D(){} // Empty constructor hidden
};

template <class KERNEL1D>
Regression1D<KERNEL1D>::Regression1D(const VECTOR& X, const VECTOR& Y)
    : Estimator1D<KERNEL1D>(X), Y_(&Y)
{}

template <class KERNEL1D> 
const VECTOR& Regression1D<KERNEL1D>::Y() const
{
    return * Y_;
}

template <class KERNEL1D>   
void Regression1D<KERNEL1D>::setY(const VECTOR& Y)
{
    Y_ = &Y;
}

template <class KERNEL1D>
const double Regression1D<KERNEL1D>::operator () (const double& x) const
{
    VECTOR::const_iterator xj = this->X_->begin();
    VECTOR::const_iterator yj = this->Y_->begin();
    double m = 0.0;
    double den = 0.0;
    for(; xj != this->X_->end() && yj != this->Y_->end(); ++yj, ++xj)
    {
        double k = this->K(x - *xj);
        m += (*yj)*k;
        den += k;
    }
    if(den < small())
        den = small();
    m /= den;
    return m;
}

template <class KERNEL1D>
const VECTOR Regression1D<KERNEL1D>::operator () (const VECTOR& X) const
{
    VECTOR M(X.size());
    VECTOR::iterator m = M.begin();
    VECTOR::const_iterator x = X.begin();
    for(; x != X.end(); ++x, ++m)
        *m = (*this)(*x);
    return M;
}




template <class KERNEL2D>
class Regression2D
    : public Estimator2D<KERNEL2D>
{
    public:
                                Regression2D(const VECTOR& X,
                                             const VECTOR& Y, 
                                             const VECTOR& Z);
                                           
        const VECTOR&           Z() const;
        const double            operator() (const double& x, 
                                            const double& y) const;
        const VECTOR            operator() (const VECTOR& X, 
                                            const VECTOR& Y) const;
        void                    setZ(const VECTOR& Z);
    
    protected:
        const VECTOR *          Z_;
        
                                Regression2D(){} // Empty constructor hidden
};

template <class KERNEL2D>
Regression2D<KERNEL2D>::Regression2D(const VECTOR& X, 
                                     const VECTOR& Y,
                                     const VECTOR& Z)
    : Estimator2D<KERNEL2D>(X, Y), Z_(&Z)
{}

template <class KERNEL2D> 
const VECTOR& Regression2D<KERNEL2D>::Z() const
{
    return * this->Z_;
}

template <class KERNEL2D>   
void Regression2D<KERNEL2D>::setZ(const VECTOR& Z)
{
    this->Z_ = &Z;
}

template <class KERNEL2D>
const double Regression2D<KERNEL2D>::operator () (const double& x,
                                                  const double& y) const
{
    VECTOR::const_iterator xj = this->X_->begin();
    VECTOR::const_iterator yj = this->Y_->begin();
    VECTOR::const_iterator zj = this->Z_->begin();
    double m = 0.0;
    double den = 0.0;
    for(; xj != this->X_->end() && yj != this->Y_->end() && zj != this->Z_->end(); ++yj, ++xj, ++zj)
    {
        double k = this->K(x - *xj, y - *yj);
        m += (*zj)*k;
        den += k;
    }
    if(den < small())
        den = small();
    m /= den;
    return m;
}

template <class KERNEL2D>
const VECTOR Regression2D<KERNEL2D>::operator () (const VECTOR& X,
                                                  const VECTOR& Y) const
{
    VECTOR M(X.size());
    VECTOR::iterator m = M.begin();
    VECTOR::const_iterator x = X.begin();
    VECTOR::const_iterator y = Y.begin();
    for(; x != X.end() && y != Y.end(); ++x, ++y, ++m)
        *m = (*this)(*x, *y);
    return M;
}


} // namespace kde

#undef VECTOR
#undef MATRIX
#endif // kde_Regression_h
