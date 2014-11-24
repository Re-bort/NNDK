#ifndef SystematicSampler_h
#define SystematicSampler_h

#include "Sampler.h"
#include "../algo/sort.h"
#include "../rng/Uniform.h"

class SystematicSampler
    : public Sampler
{
    public:
                                SystematicSampler();
                                SystematicSampler(const Dataset& dataset,
                                  const std::vector<double>& x,
                                  const double trainingFraction = 0.5,
                                  const double testingFraction = 0.25);
                                SystematicSampler(const SystematicSampler& sampler);
                                ~SystematicSampler();

        void                    sortKey(std::vector<double> x);
        virtual void            resample(void);
        
    private:
        std::vector<unsigned>   sortedKey_;
};


#endif // SystematicSampler_h
