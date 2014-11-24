#include "Encoder.h"

Encoder::~Encoder()
{
    iterator codon = codons_.begin();
    for(; codon != codons_.end(); ++codon)
    {
        delete *codon;
        *codon = NULL;
    }
}

Encoder::Codon::~Codon()
{}

Encoder::Codon& Encoder::codon(unsigned i)
{
    return *codons_[i];
} 

void Encoder::encode(std::matrix<double>& X)
{
    for(std::matrix<double>::iterator x = X.begin(); x != X.end(); ++x)
    {
        iterator codon = codons_.begin();
        std::vector<double>::iterator xi = x->begin(); 
        for(; xi != x->end(); ++xi, ++codon)
            (*codon)->encode(*xi);
    }
}

void Encoder::encode(std::vector<double>& x, unsigned i)
{
    for(std::vector<double>::iterator xi = x.begin(); xi != x.end(); ++xi)
        codon(i).encode(*xi);
}

void Encoder::decode(std::matrix<double>& X)
{
    for(std::matrix<double>::iterator x = X.begin(); x != X.end(); ++x)
    {
        iterator codon = codons_.begin();
        std::vector<double>::iterator xi = x->begin(); 
        for(; xi != x->end(); ++xi, ++codon)
            (*codon)->decode(*xi);
    }
}

void Encoder::decode(std::vector<double> x, unsigned i)
{
    for(std::vector<double>::iterator xi = x.begin(); xi != x.end(); ++xi)
        codon(i).decode(*xi);
}


