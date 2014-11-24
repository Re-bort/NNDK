#include "DbSampler.h"

#define VECTOR std::vector<double>

DbSampler::Bias::Bias(const double& val)
    : value(val)
{}

const double DbSampler::Bias::operator() (const double& x)
{
    return pow(x, value);
}

DbSampler::DbSampler()
    : density_(NULL), bias_(1.0)
{}

DbSampler::DbSampler(const Dataset& dataset,
                     const VECTOR& density,
                     const double samplingFraction,
                     const double trainingFraction,
                     const double bias)
    : Sampler(dataset, samplingFraction, trainingFraction), density_(&density),
      bias_(bias)
{}                      

DbSampler::DbSampler(const DbSampler& sampler)
    : Sampler(sampler), density_(&sampler.density()), bias_(sampler.bias()) 
{}

DbSampler::~DbSampler()
{}

const double DbSampler::bias() const
{ 
    return bias_.value;
}

const VECTOR& DbSampler::density() const
{
    return *density_;
}

void DbSampler::setBias(const double bias)
{
    bias_ = Bias(bias);
}

void DbSampler::setDensity(const VECTOR& density)
{
    density_ = &density;
}

void DbSampler::resample()
{
    // NOTE: DBS is executed twice. First, on the database indices to extract 
    // an index of the training sample.  Second, on the remaining indices to 
    // extract a testing data index.
    VECTOR proportions(density_->size());
    
    std::transform(density_->begin(),
      density_->end(),
      proportions.begin(),
      bias_);

    std::sequence<unsigned> key(0, dataset_->length());
    KEY sampleKey(sampleSize());
    std::pps_sample(key.begin(), 
      key.end(), 
      proportions.begin(), 
      sampleKey.begin(),
      sampleSize());
    
    proportions.resize(sampleSize());
    key_sample(density_->begin(), 
      sampleKey.begin(), 
      sampleKey.end(), 
      proportions.begin());

    // NOTE: to be completed.    
}

#undef VECTOR
