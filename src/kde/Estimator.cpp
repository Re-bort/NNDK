#include "Estimator.h"

#include <iostream>
#include "../util/matio.h"

#define VECTOR std::vector<double>
#define MATRIX std::matrix<double>


namespace kde {

// NOTE: Specializations of templates are explicitly instatiations of the 
// template class.  Hence, they must be placed in a .cpp file to ensure
// that they are only defined (compiled) once.  A forward declaration of
// the specialization of the template class appears in the corresponding
// header file.
// (RJM, 29/6/06)

template <> 
Estimator<Mahalanobis>::Estimator(const MATRIX& X)
    : X_(&X)
{
    setBandwidth(scottBandwidth(X.rows(), X.columns(), 1.0));
    setCovariance(X.transpose()*X);
}

template <> 
Estimator2D<Mahalanobis2D>::Estimator2D(const VECTOR& X, const VECTOR& Y)
    : X_(&X), Y_(&Y)
{
    setBandwidth(scottBandwidth(X.size(), 2.0, 1.0));
    setCovariance(var(X), var(Y), covar(X, Y));
}

} // namespace kde

#undef VECTOR
#undef MATRIX
