#ifndef DbSampler_h
#define DbSampler_h

#include "Sampler.h"

#define VECTOR std::vector<double>
#define KEY std::vector<unsigned>

class DbSampler	
    : public Sampler
{
    public:
    							DbSampler();
								DbSampler(const Dataset& dataset,
								  const VECTOR& density, 
								  const double samplingFraction = 1.0,
                                  const double trainingFraction = 0.6,
                                  const double bias = 1.0);
								DbSampler(const DbSampler& sampler);
								~DbSampler();

		const double			bias(void) const;
		const VECTOR&	        density() const;
		void 					setBias(double bias);
		void					setDensity(const VECTOR& density);

        virtual void            resample(void);
				
	private:
		const VECTOR *          density_;
        class Bias
        {
            public:
                Bias(const double& bias);
                double value;   
                const double operator() (const double& x);
        }                       bias_;
};		

// References:
// 1.   Kollios, G. et. al (2003) Efficient Biased Sampling for Approximate
//      Clustering and Outlier Detection in Large Data Sets.  IEEE 
//      Transactions on Knowledge and Data Engineering    
#undef VECTOR

#endif // DbSampler_h
