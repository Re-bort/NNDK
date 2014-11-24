#ifndef Standardiser_h
#define Standardiser_h

#include "Encoder.h"
#include "../math/statistics.h"

class Standardiser;

Standardiser standardise(std::vector<double>& x); 
Standardiser standardise(std::matrix<double>& x);

class Standardiser
    : public Encoder
{
    class Codon
        : public Encoder::Codon
    {
        public:
                                Codon();
                                Codon(const double mean_, const double stdev_);
                                Codon(const Codon& codon);
                                ~Codon();   
            void                encode(double& x);
            void                decode(double& x);
    
        private:
            double mean_;
            double stdev_;
    };
     
    public:    
                                Standardiser();
                                Standardiser(const std::vector<double>& x);
                                Standardiser(const std::matrix<double>& X);
};

#endif //Standardiser_h
