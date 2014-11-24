#include "GLRegression.h"

#define MATRIX std::matrix<double>
#define VECTOR std::vector<double>

#include "../math/statistics.h"
#include <iostream>
#include "../util/matio.h"

glr::Regression::Regression(const MATRIX& X, const VECTOR& Y)
	: X_(&X), Y_(&Y)
{
	std::cerr << " " << std::endl;
	std::cerr << "MATRIX XT = X.transpose()" << std::endl;
	MATRIX XT = X.transpose();
	std::cerr << "S_ = XT * (*X_)" << std::endl;
	S_ = XT * (*X_);
	std::cerr << S_ << std::endl;
	std::cerr << "Si_ = inverse(S_)" << std::endl;
	Si_ = inverse(S_);
	std::cerr << "B_ = Si_ * XT * (*Y_)" << std::endl;
	B_ = Si_ * XT * (*Y_);
}

const VECTOR& glr::Regression::Y() const
{
	return * Y_;
}

void glr::Regression::setY(const VECTOR& Y)
{
    Y_ = &Y;
}

const double glr::Regression::operator () (const VECTOR& x) const
												   
{
	return x * B_;
}

const VECTOR glr::Regression::operator () (const MATRIX& X) const
{
	VECTOR m = X * B_;
	return m;
}

glr::Regression1::Regression1(const VECTOR& X, const VECTOR& Y)
	: X_(&X), Y_(&Y)
{
	b_ = covar(X,Y)/var(X);
}

const VECTOR& glr::Regression1::Y() const
{
    return * Y_;
}

void glr::Regression1::setY(const VECTOR& Y)
{
    Y_ = &Y;
}

const double glr::Regression1::operator () (const double& x) const
{
    return x * b_;
}

const VECTOR glr::Regression1::operator () (const VECTOR& X) const
{
    VECTOR M(X.size());
    VECTOR::iterator m = M.begin();
    VECTOR::const_iterator x = X.begin();
    for(; x != X.end(); ++x, ++m)
        *m = (*this)(*x);
    return M;
}

#undef MATRIX
#undef VECTOR
