#include "Kernel.h"

Kernel1D::Kernel1D()
{}

Kernel1D::Kernel1D(const Kernel1D& kernel)
{}

Kernel1D::~Kernel1D()
{}

const double& Kernel1D::bandwidth() const
{
    return bandwidth_;
}

Kernel2D::Kernel2D()
{}

Kernel2D::Kernel2D(const Kernel2D& kernel)
{}

Kernel2D::~Kernel2D()
{}

const double& Kernel2D::bandwidth() const
{
    return bandwidth_;
}

Kernel::Kernel()
{}

Kernel::Kernel(const Kernel& kernel)
{}

Kernel::~Kernel()
{}

const double& Kernel::bandwidth() const
{
    return bandwidth_;
}
