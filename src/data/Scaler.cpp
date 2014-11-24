#include "Scaler.h"

Scaler::Codon::Codon(const Scaler::Codon& codon)
    : gain_(codon.gain_), offset_(codon.offset_)
{}

Scaler::Codon::Codon(const double gain, const double offset)
    : gain_(gain), offset_(offset)
{}

Scaler::Codon::~Codon()
{}

void Scaler::Codon::encode(double& x)
{
    x *= gain_;    
    x += offset_;
}

void Scaler::Codon::decode(double& x)
{
    x -= offset_;
    x /= gain_;
}

Scaler::Scaler(const std::vector<double>& x, const double a, const double b)
{
    double xmin, xmax, gain, offset;
    xmin = min(x);
    xmax = max(x);
    if (xmin == xmax)
        gain = 0.0;
    else
        gain = (b - a)/(xmax - xmin);
    offset = a - gain*xmin;
    codons_.push_back(new Codon(gain, offset));        
}

Scaler::Scaler(const std::matrix<double>& X, const double a, const double b)
{
    double c, d, gain, offset;
    for(unsigned j = 0; j != X.columns(); ++j)
    {
        std::vector<double> x = X.column(j);
        c = min(x);
        d = max(x);
        if (c == d)
            gain = 0.0;
        else
            gain = (b - a)/(d - c);
        offset = a - c*gain;
        codons_.push_back(new Codon(gain, offset));
    }
}

Scaler scale(std::vector<double>& x, const double a, const double b)
{
    Scaler scaler(x, a, b);
    scaler.encode(x);
    return scaler;
} 

Scaler scale(std::matrix<double>& X, const double a, const double b)
{
    Scaler scaler(X, a, b);
    scaler.encode(X);
    return scaler;
}
