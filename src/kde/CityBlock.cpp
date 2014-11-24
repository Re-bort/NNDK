#include "CityBlock.h"

#include <math.h>

#include "../math/vecops.h"

#define VECTOR std::vector<double>

CityBlock1D::CityBlock1D()
{}

CityBlock1D::~CityBlock1D()
{}

CityBlock1D::CityBlock1D(const double& bandwidth)
{
    setBandwidth(bandwidth);
}

void CityBlock1D::setBandwidth(const double& bandwidth)
{
    bandwidth_ = bandwidth;
    scaledBandwidth_ = 2.0*bandwidth_;
}

const double CityBlock1D::width() const
{
    return 2.0*bandwidth_;  
}

const double CityBlock1D::operator () (const double& x) const
{
    return exp( -fabs(x) / scaledBandwidth_);
}

CityBlock2D::CityBlock2D()
{}

CityBlock2D::CityBlock2D(const double& bandwidth)
{
    setBandwidth(bandwidth);
}

CityBlock2D::~CityBlock2D()
{}

void CityBlock2D::setBandwidth(const double& bandwidth)
{
    bandwidth_ = bandwidth;
    scaledBandwidth_ = 2.0*bandwidth_;
}

const double CityBlock2D::volume() const
{
    return pow(2.0*bandwidth_, 2.0);  
}

const double CityBlock2D::operator () (const double& x, 
                                              const double& y) const
{
    return exp(-(fabs(x) + fabs(y)) / scaledBandwidth_);
}

CityBlock::CityBlock()
{}

CityBlock::CityBlock(const double& bandwidth)
{
    setBandwidth(bandwidth);
}

CityBlock::~CityBlock()
{}

void CityBlock::setBandwidth(const double& bandwidth)
{
    bandwidth_ = bandwidth;
    scaledBandwidth_ = 2.0*bandwidth_;
}

const double CityBlock::volume(const unsigned& d) const
{
    return pow(2.0*bandwidth_, (double)d);  
}

const double CityBlock::operator () (const VECTOR& x) const
{
    return exp(-manhattan(x) / scaledBandwidth_);
}

#undef VECTOR
