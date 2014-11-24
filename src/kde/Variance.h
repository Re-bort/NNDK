#ifndef kde_Variance_h
#define kde_Variance_h

#include "Estimator.h"
#include "Regression.h"
#include "../math/constants.h"

#define VECTOR std::vector<double>
#define MATRIX std::matrix<double>
#define REGRESSION kde::Regression<KERNEL>

namespace kde {
	
template <class KERNEL>
class Variance
    : public Estimator<KERNEL>
{
	public:
						        Variance(const REGRESSION& m);
										   
		const double			operator() (const VECTOR& x) const;
		const VECTOR	        operator() (const MATRIX& X) const;
		const VECTOR			leaveOneOut() const;
	           
	protected:
	
		const MATRIX *			X_;
		const VECTOR *			Y_;
	
		const REGRESSION*		m_;
                                Variance(){} // Empty constructor hidden
};

template <class KERNEL>
Variance<KERNEL>::Variance(const REGRESSION& m)
	: Estimator<KERNEL>(m.X()), X_(&m.X()), Y_(&m.Y()), m_(&m)
{
	this->setBandwidth(m.bandwidth());
}

template <class KERNEL>
const double Variance<KERNEL>::operator () (const VECTOR& x) const
												   
{
	MATRIX::const_iterator xj = this->X_->begin();
	VECTOR::const_iterator yj = this->Y_->begin();
	double v = 0.0;
	double den = 0.0;
	double m = (*m_)(x);
	//double m = 0.05;
	for(; xj != this->X_->end() && yj != this->Y_->end(); ++yj, ++xj)
	{
		double aj = this->K(x - *xj);
		v += pow(*yj - m, 2.0) * aj;
		den += aj;
	}
	if(den < small())
		den = small();
	v /= den;
	return v;
}

template <class KERNEL>
const VECTOR Variance<KERNEL>::operator () (const MATRIX& X) const
{
	VECTOR V(X.rows());
    VECTOR::iterator v = V.begin();
    MATRIX::const_iterator x = X.begin();
    for(; x != X.end(); ++x, ++v)
       *v = (*this)(*x);
    return V;
}

template <class KERNEL>
const VECTOR Variance<KERNEL>::leaveOneOut() const
{
    VECTOR V(this->X_->rows());
    VECTOR::iterator v = V.begin();
    MATRIX::const_iterator x = this->X_->begin();
    for(; x != this->X_->end(); ++x, ++v)
    {
        *v = 0.0;
        double den = 0.0;
    	double m = (*(this->m_))(*x);
    	MATRIX::const_iterator xj = X_->begin();
        VECTOR::const_iterator yj = Y_->begin();
        for(; xj != x && yj != this->Y_->end(); ++xj, ++yj)
        {
            double aj = this->K(*x - *xj);
            *v += pow(*yj - m, 2.0) * aj;
            den += aj;
        }
        ++xj;   // Leave out case of *xj = x
        ++yj;
        for(; xj != this->X_->end() && yj != this->Y_->end(); ++xj, ++yj)
        {
            double aj = this->K(*x - *xj);
            *v += pow(*yj - m, 2.0) * aj;
            den += aj;
        }
        if(den < small())
            den = small();
        *v /= den;
    }
    return V;
}
        
//template <class KERNEL1D>
//class Variance1D
//    : public Estimator1D<KERNEL1D>
//{
//    public:
//                                Variance1D(const VECTOR& X, const VECTOR& Y);
//                                           
//        const VECTOR&           Y() const;
//        const double            operator() (const double& x) const;
//        const VECTOR            operator() (const VECTOR& X) const;
//        void                    setY(const VECTOR& Y);
//    
//    protected:
//        const VECTOR *          Y_;
//        
//                                Variance1D(){} // Empty constructor hidden
//};
//
//template <class KERNEL1D>
//Variance1D<KERNEL1D>::Variance1D(const VECTOR& X, const VECTOR& Y)
//    : Estimator1D<KERNEL1D>(X), Y_(&Y)
//{}
//
//template <class KERNEL1D> 
//const VECTOR& Variance1D<KERNEL1D>::Y() const
//{
//    return * Y_;
//}
//
//template <class KERNEL1D>   
//void Variance1D<KERNEL1D>::setY(const VECTOR& Y)
//{
//    Y_ = &Y;
//}
//
//template <class KERNEL1D>
//const double Variance1D<KERNEL1D>::operator () (const double& x) const
//{
//    VECTOR::const_iterator xj = X_->begin();
//    VECTOR::const_iterator yj = Y_->begin();
//    double m = 0.0;
//    double den = 0.0;
//    for(; xj != X_->end() && yj != Y_->end(); ++yj, ++xj)
//    {
//        double k = K(x - *xj);
//        m += (*yj)*k;
//        den += k;
//    }
//    if(den < small())
//        den = small();
//    m /= den;
//    return m;
//}
//
//template <class KERNEL1D>
//const VECTOR Variance1D<KERNEL1D>::operator () (const VECTOR& X) const
//{
//    VECTOR M(X.size());
//    VECTOR::iterator m = M.begin();
//    VECTOR::const_iterator x = X.begin();
//    for(; x != X.end(); ++x, ++m)
//        *m = (*this)(*x);
//    return M;
//}
//
//template <class KERNEL2D>
//class Variance2D
//    : public Estimator2D<KERNEL2D>
//{
//    public:
//                                Variance2D(const VECTOR& X,
//                                             const VECTOR& Y, 
//                                             const VECTOR& Z);
//                                           
//        const VECTOR&           Z() const;
//        const double            operator() (const double& x, 
//                                            const double& y) const;
//        const VECTOR            operator() (const VECTOR& X, 
//                                            const VECTOR& Y) const;
//        void                    setZ(const VECTOR& Z);
//    
//    protected:
//        const VECTOR *          Z_;
//        
//                                Variance2D(){} // Empty constructor hidden
//};
//
//template <class KERNEL2D>
//Variance2D<KERNEL2D>::Variance2D(const VECTOR& X, 
//                                     const VECTOR& Y,
//                                     const VECTOR& Z)
//    : Estimator2D<KERNEL2D>(X, Y), Z_(&Z)
//{}
//
//template <class KERNEL2D> 
//const VECTOR& Variance2D<KERNEL2D>::Z() const
//{
//    return * Z_;
//}
//
//template <class KERNEL2D>   
//void Variance2D<KERNEL2D>::setZ(const VECTOR& Z)
//{
//    Z_ = &Z;
//}
//
//template <class KERNEL2D>
//const double Variance2D<KERNEL2D>::operator () (const double& x,
//                                                  const double& y) const
//{
//    VECTOR::const_iterator xj = X_->begin();
//    VECTOR::const_iterator yj = Y_->begin();
//    VECTOR::const_iterator zj = Z_->begin();
//    double m = 0.0;
//    double den = 0.0;
//    for(; xj != X_->end() && yj != Y_->end() && zj != Z_->end(); ++yj, ++xj, ++zj)
//    {
//        double k = K(x - *xj, y - *yj);
//        m += (*zj)*k;
//        den += k;
//    }
//    if(den < small())
//        den = small();
//    m /= den;
//    return m;
//}
//
//template <class KERNEL2D>
//const VECTOR Variance2D<KERNEL2D>::operator () (const VECTOR& X,
//                                                  const VECTOR& Y) const
//{
//    VECTOR M(X.size());
//    VECTOR::iterator m = M.begin();
//    VECTOR::const_iterator x = X.begin();
//    VECTOR::const_iterator y = Y.begin();
//    for(; x != X.end() && y != Y.end(); ++x, ++y, ++m)
//        *m = (*this)(*x, *y);
//    return M;
//}


} // namespace kde

#undef VECTOR
#undef MATRIX
#endif // kde_Variance_h
