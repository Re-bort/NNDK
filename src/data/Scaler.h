#ifndef Scaler_h
#define Scaler_h

#include "Encoder.h"
#include "../math/statistics.h"

class Scaler;

Scaler scale(std::vector<double>& x, 
			 const double a = 0.0, 
			 const double b = 1.0);
Scaler scale(std::matrix<double>& x, 
             const double a = 0.0, 
             const double b = 1.0);
    
class Scaler
    : public Encoder
{
    class Codon
        : public Encoder::Codon
    {
        public:
                                Codon(const double gain_, const double offset_);   
                                Codon(const Codon& codon);
                                ~Codon();
                                
                                
            void                encode(double& x);
            void                decode(double& x);
    
        private:
            double gain_;
            double offset_;
    };
     
    public:    
                                Scaler(const std::vector<double>& x,
                                       const double a = 0.0,
                                       const double b = 1.0);
                                Scaler(const std::matrix<double>& X,
                                       const double a = 0.0,
                                       const double b = 1.0);
};

#endif // Scaler_h

