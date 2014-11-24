#include "Mahalanobis.h"

#include "../math/vecops.h"
#include "../math/matops.h"
#include "../math/constants.h"

#define VECTOR std::vector<double>
#define MATRIX std::matrix<double>

Mahalanobis1D::Mahalanobis1D()
{}

Mahalanobis1D::Mahalanobis1D(const double& bandwidth)
{
    setBandwidth(bandwidth);
}

Mahalanobis1D::~Mahalanobis1D()
{}

void Mahalanobis1D::setBandwidth(const double& bandwidth)
{
    bandwidth_ = bandwidth;
    scaledBandwidth_ = 2.0 * bandwidth_ * bandwidth_;
}

const double Mahalanobis1D::width() const
{
    return sqrt(2.0 * pi()) * bandwidth_;     
}

const double Mahalanobis1D::operator () (const double& x) const
{
    return exp( -(x * x) / scaledBandwidth_);
}

Mahalanobis2D::Mahalanobis2D()
{
    bandwidth_ = 1.0;
    inverseCovariance_ = std::identity<double>(2);
    determinant_ = 1.0;
}

Mahalanobis2D::Mahalanobis2D(const double& bandwidth, 
                     const double& varx,
                     const double& vary,
                     const double& varxy)
{
    setBandwidth(bandwidth);
    setCovariance(varx, vary, varxy);
}

Mahalanobis2D::~Mahalanobis2D()
{}

void Mahalanobis2D::setBandwidth(const double& bandwidth)
{
    bandwidth_ = bandwidth;
    scaledBandwidth_ = 2.0 * bandwidth_ * bandwidth_;
}

void Mahalanobis2D::setCovariance(const double& varx,
                                  const double& vary,
                                  const double& varxy)
{
    determinant_ = varx * vary - varxy * varxy;
    inverseCovariance_[0][0] = varx / determinant_;
    inverseCovariance_[1][1] = vary / determinant_;
    inverseCovariance_[0][1] = -varxy / determinant_;
    inverseCovariance_[1][0] = -varxy / determinant_;
}

const double Mahalanobis2D::volume() const
{
    return sqrt(determinant_) * pow(sqrt(2.0 * pi()) * bandwidth_, 2.0);     
}

const double Mahalanobis2D::d(const double& x, const double& y) const
{
//      const MATRIX& Si_ = inverseCovariance_;
//      return x*(x*Si_[0][0] + y*Si_[1][0]*Si_[1][0])
//         + y*(x*Si_[0][1] + y*Si_[1][1]);
//    return x * (x * inverseCovariance_[0][0] + y * inverseCovariance_[1][0])
//         + y * (x * inverseCovariance_[0][1] + y * inverseCovariance_[1][1]); 
    return x * x * inverseCovariance_[0][0] 
        + y * x * (inverseCovariance_[1][0]  +  inverseCovariance_[0][1])
        + y * y * inverseCovariance_[1][1];
}

const double Mahalanobis2D::operator () (const double& x,
                                         const double& y) const
{
    return exp(-d(x, y) / scaledBandwidth_);
}

Mahalanobis::Mahalanobis()
{}

Mahalanobis::Mahalanobis(const double& bandwidth, const MATRIX& covariance)
{
    setBandwidth(bandwidth);
    setCovariance(covariance);
}

Mahalanobis::~Mahalanobis()
{}

void Mahalanobis::setBandwidth(const double& bandwidth)
{
    bandwidth_ = bandwidth;
    scaledBandwidth_ = 2.0 * bandwidth_ * bandwidth_;
}

void Mahalanobis::setCovariance(const MATRIX& covariance)
{
    inverseCovariance_ = inverse(covariance);   
    determinant_ = determinant(covariance);
}

const double Mahalanobis::volume(const unsigned& d) const
{
    return sqrt(determinant_) * pow(sqrt(2.0 * pi()) * bandwidth_,(double)d);     
}

const double Mahalanobis::operator () (const VECTOR& x) const
{
    return exp(-(x * inverseCovariance_ * x) / scaledBandwidth_);
}

#undef VECTOR
#undef MATRIX
