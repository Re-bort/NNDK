#include "Sampler.h"

#define TABLE std::table<double> 
#define MATRIX std::matrix<double>
#define SERIES std::series<double>
#define VECTOR std::vector<double>
#define KEY std::vector<unsigned>

Sampler::Sampler()
    : trainingFraction_(0.5), testingFraction_(0.25)
{}

Sampler::Sampler(const Dataset& dataset,
           const double trainingFraction,
           const double testingFraction)
    : trainingFraction_(trainingFraction), testingFraction_(testingFraction)
{
    setDataset(dataset);
}

Sampler::Sampler(const Sampler& sampler)
    : trainingFraction_(sampler.trainingFraction()),
    testingFraction_(sampler.testingFraction())
{
    setDataset(sampler.dataset());
}

Sampler::~Sampler()
{
    dataset_ = NULL;
}

const Dataset& Sampler::dataset() const
{
    return * dataset_;
}

const double Sampler::testingFraction() const
{
    return testingFraction_;
}

const double Sampler::trainingFraction() const
{
    return trainingFraction_;
}

const double Sampler::validatingFraction() const
{
    return 1.0 - trainingFraction_ - testingFraction_;    
}

void Sampler::setDataset(const Dataset& dataset)
{
    dataset_ = &dataset;
    training_.X.header = dataset.X.header;
    training_.X.index.label = dataset.X.index.label;
    training_.y.label = dataset.y.label;
    testing_.X.header = dataset.X.header;
    testing_.X.index.label = dataset.X.index.label;
    testing_.y.label = dataset.y.label;
    validating_.X.header = dataset.X.header;
    validating_.X.index.label = dataset.X.index.label;
    validating_.y.label = dataset.y.label;
}
    
void Sampler::setTestingFraction(const double testingFraction)
{
    testingFraction_ = testingFraction;
}

void Sampler::setTrainingFraction(const double trainingFraction)
{
    trainingFraction_ = trainingFraction;
}

const Dataset& Sampler::testing() const
{
    return testing_;    
}

const Dataset& Sampler::training() const
{
    return training_;
}

const Dataset& Sampler::validating() const
{
    return validating_;
}

const KEY&	Sampler::trainingIndex() const
{
	return trainingKey_;
}

const KEY& Sampler::testIndex() const
{
	return testingKey_;
}

const KEY& Sampler::validatingIndex() const
{
	return validatingKey_;
}


void Sampler::resample()
{
    std::sequence<unsigned> key(0, dataset_->length());
    
    std::random_shuffle(key.begin(), key.end());
    trainingKey_.resize(trainingSampleSize());
    block_sample(key.begin(), 
      trainingKey_.begin(), 
      trainingSampleSize());
    allocate(trainingKey_, training_);
    
    testingKey_.resize(testingSampleSize());
    block_sample(key.begin() + trainingSampleSize(),
      testingKey_.begin(),
      testingSampleSize());
    allocate(testingKey_, testing_);
      
    validatingKey_.resize(validatingSampleSize());
    block_sample(key.begin() + trainingSampleSize() + testingSampleSize(),
      validatingKey_.begin(),
      validatingSampleSize());
    allocate(validatingKey_, validating_);
}

void Sampler::allocate(Dataset& sample)
{
	allocate(trainingKey_, training_);
	allocate(testingKey_, testing_);
	allocate(validatingKey_, validating_);
}

void Sampler::allocate(KEY key, Dataset& sample)
{
    sample.X.resize(key.size());
    sample.y.resize(key.size());
    key_sample(dataset_->X.index.begin(),
      key.begin(), 
      key.end(),
      sample.X.index.begin());
    key_sample(dataset_->X.begin(),
      key.begin(), 
      key.end(),
      sample.X.begin());
    key_sample(dataset_->y.begin(),
      key.begin(), 
      key.end(),
      sample.y.begin());
}

const unsigned Sampler::sampleSize() const
{
    return (unsigned)(dataset_->length() * 
    	(trainingFraction_ + testingFraction_) + 0.5);
    // KLUDGE: Casting to unsigned from double may lose a single bit due to 
    // loss of sign, which may reduce returned value by 1. Value of 0.5 is
    // added after multiplication, and prior to casting to ensure the correct
    // unsigned value is returned.  This also ensures correct rounding of
    // double to nearest integer.
}

const unsigned Sampler::trainingSampleSize() const
{
    return (unsigned)(dataset_->length() * trainingFraction_ + 0.5);
}

const unsigned Sampler::testingSampleSize() const
{
    return (unsigned)(dataset_->length() * testingFraction_ + 0.5);
}

const unsigned Sampler::validatingSampleSize() const
{
    return (unsigned)(dataset_->length() - trainingSampleSize()
    	- testingSampleSize());
}

#undef TABLE
#undef MATRIX
#undef VECTOR
#undef SERIES
#undef KEY 

