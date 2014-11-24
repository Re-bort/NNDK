#ifndef kde_Estimator_h
#define kde_Estimator_h

#include "Gaussian.h"
#include "CityBlock.h"
#include "Mahalanobis.h"
#include "Scott.h"
#include "../math/statistics.h"

#define VECTOR std::vector<double>
#define MATRIX std::matrix<double>

namespace kde {

template <class KERNEL>
class Estimator
    : public KERNEL
{
    public:
                                Estimator(const MATRIX& X);
                                Estimator(const Estimator<KERNEL>& estimator);
        virtual const double    operator () (const VECTOR& x) const = 0;
        virtual const VECTOR    operator () (const MATRIX& X) const = 0; 
        const MATRIX&           X(void) const;
        
    protected:
        const MATRIX *          X_;    

                                Estimator() {} // Empty constructor hidden
        const double            K(const VECTOR& x) const;
};

template <class KERNEL>
Estimator<KERNEL>::Estimator(const Estimator<KERNEL>& estimator)
	: KERNEL(estimator), X_(&estimator.X())
{}

template <class KERNEL>
Estimator<KERNEL>::Estimator(const MATRIX& X)
    : X_(&X)
{
    KERNEL::setBandwidth(scottBandwidth(X.rows(), X.columns(), 1.0));
}

template <> 
Estimator<Mahalanobis>::Estimator(const MATRIX& X);

template <class KERNEL>
inline const MATRIX& Estimator<KERNEL>::X() const
{
    return *X_;
}

template <class KERNEL>
inline const double Estimator<KERNEL>::K(const VECTOR& x) const
{
    return KERNEL::operator () (x);
}


template <class KERNEL1D>
class Estimator1D
    : public KERNEL1D
{
    public:
                                Estimator1D(const VECTOR& X);
        virtual const double    operator () (const double& x) const = 0;
        virtual const VECTOR    operator () (const VECTOR& X) const = 0; 
        const VECTOR&           X(void) const;
    
    protected:
        const VECTOR *          X_;    

                                Estimator1D() {} // Empty constructor hidden
        const double            K(const double& x) const;
};

template <class KERNEL1D>
Estimator1D<KERNEL1D>::Estimator1D(const VECTOR& X)
    : X_(&X)
{
    KERNEL1D::setBandwidth(scottBandwidth(X.size(), 1, 1.0));
}

template <class KERNEL1D>
inline const VECTOR& Estimator1D<KERNEL1D>::X() const
{
    return * X_;
}

template <class KERNEL1D>
inline const double Estimator1D<KERNEL1D>::K(const double& x) const
{
    return KERNEL1D::operator () (x);
}


template <class KERNEL2D>
class Estimator2D
    : public KERNEL2D
{
    public:
                                Estimator2D(const VECTOR& X, const VECTOR& Y);
        virtual const double    operator () (const double& x, 
                                  const double& y) const = 0;
        virtual const VECTOR    operator () (const VECTOR& X, 
                                  const VECTOR& Y) const = 0; 
        const VECTOR&           X(void) const;
        const VECTOR&           Y(void) const;
    
    protected:
        const VECTOR *          X_;    
        const VECTOR *          Y_;

                                Estimator2D() {} // Empty constructor hidden
        const double            K(const double& x, const double& y) const;
};

template <class KERNEL2D>
Estimator2D<KERNEL2D>::Estimator2D(const VECTOR& X, const VECTOR& Y)
    : X_(&X), Y_(&Y)
{
    KERNEL2D::setBandwidth(scottBandwidth(X.size(), 2.0, 1.0));
}

template <> 
Estimator2D<Mahalanobis2D>::Estimator2D(const VECTOR& X, const VECTOR& Y);


template <class KERNEL2D>
inline const VECTOR& Estimator2D<KERNEL2D>::X() const
{
    return * X_;
}

template <class KERNEL2D>
inline const VECTOR& Estimator2D<KERNEL2D>::Y() const
{
    return * Y_;
}

template <class KERNEL2D>
inline const double Estimator2D<KERNEL2D>::K(const double& x, 
                                             const double& y) const
{
    return KERNEL2D::operator () (x, y);
}

} // namespace kde

#undef VECTOR
#undef MATRIX

#endif // kde_Estimator_h
