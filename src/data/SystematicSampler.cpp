#include "SystematicSampler.h"

#define TABLE std::table<double> 
#define MATRIX std::matrix<double>
#define SERIES std::series<double>
#define VECTOR std::vector<double>
#define KEY std::vector<unsigned>


// Returns systematic sample of k-th value of x, where k is determined
// by the fraction argument (k = 1/f). 
const KEY sample(const KEY& x, const double fraction)
{
    KEY sample;
    double interval = 1.0 / fraction;
    // Randomly select starting location on interval [0, m]
    // where m ~ interval size.
    unsigned m = (unsigned)ceil(interval);
    double location = uniform((unsigned)0, m);
    for(; location < x.size() - 1; location += interval)
    {
        // For non-integer locations, round up to next integer value k
        unsigned k = (unsigned)ceil(location);
        // Take k-th sample
        sample.push_back(x[k]);          
    }
    return sample;
}

// Returns list of key values of x NOT in y (i.e. set_difference)
const KEY difference(const KEY& x, const KEY& y)
{
    KEY difference;
    KEY::const_iterator xi = x.begin(), yi = y.begin();
    for(; xi != x.end(); ++xi)
    {
        if(*xi == *yi) // Found yi in x, move to next yi to look for;
            ++yi;
        else 
            difference.push_back(*xi);     
    }
    return difference;
}

SystematicSampler::SystematicSampler()
{}

SystematicSampler::SystematicSampler(const Dataset& dataset,
                                  const VECTOR& x,
                                  const double trainingFraction,
                                  const double testingFraction)
    : Sampler(dataset, trainingFraction, testingFraction)
  {
        sortKey(x);
  }

SystematicSampler::SystematicSampler(const SystematicSampler& sampler)
    : Sampler(sampler), sortedKey_(sampler.sortedKey_)
{}

SystematicSampler::~SystematicSampler()
{}


void SystematicSampler::sortKey(VECTOR x)
{
    sortedKey_ =  std::sequence<unsigned> (0, x.size());
    std::sort(x, sortedKey_);
}

void SystematicSampler::resample()
{
    trainingKey_.clear();
    testingKey_.clear();
    validatingKey_.clear();
    
    double calibratingFraction = trainingFraction_ + testingFraction_;
    
    // Split into systematic sample and out-of-sample
    KEY sampleKey = sample(sortedKey_, calibratingFraction);
    validatingKey_ = difference(sortedKey_, sampleKey); 
    
    // Split sample into systematic testing and training samples
    testingKey_ = sample(sampleKey, testingFraction() / calibratingFraction);
    trainingKey_ = difference(sampleKey, testingKey_);

    allocate(trainingKey_, training_);
    allocate(testingKey_, testing_);
    allocate(validatingKey_, validating_);
}

#undef TABLE
#undef MATRIX
#undef VECTOR
#undef SERIES
#undef KEY 
