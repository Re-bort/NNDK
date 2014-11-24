#include "Gaussian.h"

#define VECTOR std::vector<double>

Gaussian1D::Gaussian1D()
{}

Gaussian1D::~Gaussian1D()
{}

Gaussian1D::Gaussian1D(const double& bandwidth)
{
    setBandwidth(bandwidth);
}   

void Gaussian1D::setBandwidth(const double& bandwidth)
{
    bandwidth_ = bandwidth;
    scaledBandwidth_ = 2.0 * bandwidth_ * bandwidth_;
}

const double Gaussian1D::width() const
{
    return sqrt(2.0 * pi()) * bandwidth_;
}

const double Gaussian1D::operator () (const double& x) const
{
    return exp(-(x * x) / scaledBandwidth_);
}

Gaussian2D::Gaussian2D()
{}

Gaussian2D::Gaussian2D(const double& bandwidth)
{
    setBandwidth(bandwidth);
}   

Gaussian2D::~Gaussian2D()
{}

void Gaussian2D::setBandwidth(const double& bandwidth)
{
    bandwidth_ = bandwidth;
    scaledBandwidth_ = 2.0 * bandwidth_ * bandwidth_;
}

const double Gaussian2D::volume() const
{
    return pow(sqrt(2.0 * pi()) * bandwidth_, 2.0);
}

const double Gaussian2D::operator () (const double& x, const double& y) const
{
    return exp(-(x * x + y * y) / scaledBandwidth_);
}

Gaussian::Gaussian()
{}

Gaussian::Gaussian(const double& bandwidth)
{
    setBandwidth(bandwidth);
}   

Gaussian::~Gaussian()
{}

void Gaussian::setBandwidth(const double& bandwidth)
{
    bandwidth_ = bandwidth;
    scaledBandwidth_ = 2.0 * bandwidth_ * bandwidth_;
}

const double Gaussian::volume(const unsigned& d) const
{
    return pow(sqrt(2.0 * pi()) * bandwidth_, (double)d);
}

const double Gaussian::operator () (const VECTOR& x) const
{
    return exp(-(x * x) / scaledBandwidth_);
}

#undef VECTOR

