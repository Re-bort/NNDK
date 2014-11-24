#ifndef kde_Pdf_h
#define kde_Pdf_h

#include "Estimator.h"

#define VECTOR std::vector<double>
#define MATRIX std::matrix<double>

namespace kde {

template <class KERNEL>
class Pdf
	: public Estimator<KERNEL>, public VECTOR
{
    public:
		                        Pdf(const MATRIX& X);
                                ~Pdf(){}
									
		virtual const double    operator () (const VECTOR& x) const;
		virtual const VECTOR	operator () (const MATRIX& X) const;
       
    private:				
                                Pdf(){} // Empty constructor hidden
};

template <class KERNEL>
Pdf<KERNEL>::Pdf(const MATRIX& X)
    : Estimator<KERNEL>(X), VECTOR(X.rows(), 0.0)
{
    VECTOR::iterator fi = this->begin();
    MATRIX::const_iterator Xi = X.begin();
    for(; Xi != X.end(); ++Xi, ++fi)
        *fi = (*this)(*Xi);
}

template <class KERNEL>
const double Pdf<KERNEL>::operator () (const VECTOR& x) const
{
	double f = 0.0;
    MATRIX::const_iterator xj = this->X_->begin();
	for(; xj != this->X_->end(); ++xj)
    {
        f += this->K(x - *xj);
    }
	f /= (double)this->X_->rows() * this->volume(this->X_->columns());
	return f;
}

template <class KERNEL>
const VECTOR Pdf<KERNEL>::operator () (const MATRIX& X) const
{
	VECTOR f(X.rows());
	VECTOR::iterator fi = f.begin();
	MATRIX::const_iterator x = X.begin();
	for(; x != X.end(); ++x, ++fi)
		*fi = (*this)(*x);
	return f;
}

template <class KERNEL1D>
class Pdf1D
    : public Estimator1D<KERNEL1D>, public VECTOR
{
    public:
                                Pdf1D(const VECTOR& X);
                                ~Pdf1D(){}
                                    
        virtual const double    operator () (const double& x) const;
        virtual const VECTOR    operator () (const VECTOR& X) const;
               
    private:                
                                Pdf1D(){} // Empty constructor hidden
};

template <class KERNEL1D>
Pdf1D<KERNEL1D>::Pdf1D(const VECTOR& X)
    : Estimator1D<KERNEL1D>(X), VECTOR(X.size(), 0.0)
{
    VECTOR::iterator fi = this->begin();
    VECTOR::const_iterator xi = X.begin();
    for(; xi != X.end(); ++xi, ++fi)
        *fi = (*this)(*xi);
}

template <class KERNEL1D> 
const double Pdf1D<KERNEL1D>::operator () (const double& x) const
{
    double f = 0.0;
    VECTOR::const_iterator xj = this->X_->begin();
    for(; xj != this->X_->end(); ++xj)
        f += this->K(x - *xj);
    f /= (this->X_->size() * this->width());
    return f;
}

template <class KERNEL1D>
const VECTOR Pdf1D<KERNEL1D>::operator () (const VECTOR& X) const
{
    VECTOR f(X.size());
    VECTOR::iterator fi = f.begin();
    VECTOR::const_iterator x = X.begin();
    for(; x != X.end(); ++x, ++fi)
        *fi = (*this)(*x);
    return f;
}


template <class KERNEL2D>
class Pdf2D
    : public Estimator2D<KERNEL2D>, public VECTOR
{
    public:
                                Pdf2D(const VECTOR& X, const VECTOR& Y);
                                ~Pdf2D(){}
                                    
        virtual const double    operator () (const double& x,
                                             const double& y) const;
        virtual const VECTOR    operator () (const VECTOR& X,
                                             const VECTOR& Y) const;
               
    private:                
                                Pdf2D(){} // Empty constructor hidden
};

template <class KERNEL2D>
Pdf2D<KERNEL2D>::Pdf2D(const VECTOR& X, const VECTOR& Y)
    : Estimator2D<KERNEL2D>(X, Y), VECTOR(X.size(), 0.0)
{
    VECTOR::iterator fi = begin();
    VECTOR::const_iterator xi = X.begin();
    VECTOR::const_iterator yi = Y.begin();
    for(; xi != X.end(); ++xi, ++yi, ++fi)
        *fi = (*this)(*xi, *yi);
}


template <class KERNEL2D> 
const double Pdf2D<KERNEL2D>::operator () (const double& x, 
                                           const double& y) const
{
    double f = 0.0;
    VECTOR::const_iterator xj = this->X_->begin();
    VECTOR::const_iterator yj = this->Y_->begin();
    for(; xj != this->X_->end() && yj != this->Y_->end(); ++xj, ++yj)
        f += this->K(x - *xj, y - *yj);
    f /= (this->X_->size() * this->volume());
    return f;
}

template <class KERNEL2D>
const VECTOR Pdf2D<KERNEL2D>::operator () (const VECTOR& X, 
                                           const VECTOR& Y) const
{
    VECTOR F(X.size());
    VECTOR::iterator f = F.begin();
    VECTOR::const_iterator x = X.begin();
    VECTOR::const_iterator y = Y.begin();
    for(; x != X.end() && y != Y.end(); ++x, ++y, ++f)
        *f = (*this)(*x, *y);
    return F;
}

} // namespace kde

#undef VECTOR
#undef MATRIX
#endif // kde_Pdf_h
