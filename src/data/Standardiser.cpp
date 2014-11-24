#include "Standardiser.h"

Standardiser::Codon::Codon(const double mean, const double stdev)
    : mean_(mean), stdev_(stdev)
{}

Standardiser::Codon::Codon(const Standardiser::Codon& codon)
    : mean_(codon.mean_), stdev_(codon.stdev_)
{}

Standardiser::Codon::~Codon()
{}


void Standardiser::Codon::encode(double& x)
{
    x -= mean_;    
    x /= stdev_;
}

void Standardiser::Codon::decode(double& x)
{
    x *= stdev_;
    x += mean_; 
}

Standardiser::Standardiser(const std::vector<double>& x)
{
    codons_.push_back(new Codon(::ave(x), ::stdev(x)));        
}

Standardiser::Standardiser(const std::matrix<double>& X)
{
    for(unsigned j = 0; j != X.columns(); ++j)
    {
        std::vector<double> x = X.column(j);
        codons_.push_back(new Codon(::ave(x), ::stdev(x)));
    }
}

Standardiser standardise(std::vector<double>& x)
{
    Standardiser standardiser(x);
    standardiser.encode(x);
    return standardiser;
} 

Standardiser standardise(std::matrix<double>& X)
{
    Standardiser standardiser(X);
    standardiser.encode(X);
    return standardiser;
}
