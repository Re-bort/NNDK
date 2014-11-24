#include "Grnn.h"

#define MATRIX std::matrix<double>
#define VECTOR std::vector<double>

Grnn::Grnn(const MATRIX& X, const VECTOR& y)
    : kde::Regression<Gaussian>(X, y) 
{}

VarGrnn::VarGrnn(const Grnn& grnn)
    : kde::Variance<Gaussian>(grnn) 
{}

#undef MATRIX
#undef VECTOR

