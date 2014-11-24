#include "SomSampler.h"
#include <iostream>

#define KEY std::vector<unsigned>

SomSampler::SomSampler()
	: outlierThreshold_(3)
{}

SomSampler::SomSampler(const Dataset& dataset, 
                       const Som& som, 
                       const double trainingFraction,
                       const double testingFraction)
    : Sampler(dataset, trainingFraction, testingFraction), som_(&som), outlierThreshold_(3)
{}

SomSampler::SomSampler(const SomSampler& sampler)
    : Sampler(sampler), som_(sampler.som_), outlierThreshold_(sampler.outlierThreshold_)
{}

SomSampler::~SomSampler()
{
   som_ = NULL;   
}

void SomSampler::setOutlierThreshold(const unsigned& outlierThreshold)
{
	outlierThreshold_ = outlierThreshold;	
}

void SomSampler::resample()
{
    trainingKey_.clear();
    testingKey_.clear();
    validatingKey_.clear();
    
    for(unsigned i = 0; i < som_->rows(); ++i)
    {
        for(unsigned j = 0; j < som_->columns(); ++j)
        {
            const Som::Neuron& neuron = som_->neuron(i,j);
            
            if (!neuron.empty())
            {
                KEY::const_iterator key = neuron.indices().begin();                               
                trainingKey_.push_back(*key);
                ++key; 
                if (key != neuron.indices().end()) 
                {
                    testingKey_.push_back(*key);
                    ++key;
                }
                while(key != neuron.indices().end())
                {
                    validatingKey_.push_back(*key);
                    ++key;
                }
            }
        }    
    }
    
    allocate(trainingKey_, training_);
    allocate(testingKey_, testing_);
    allocate(validatingKey_, validating_);
}

void SomSampler::resample(std::matrix<double> sizes)
{
    trainingKey_.clear();
    testingKey_.clear();
    validatingKey_.clear();
    
    normalize(sizes);

    for(unsigned i = 0; i < som_->rows(); ++i)
    {
        for(unsigned j = 0; j < som_->columns(); ++j)
        {
            const Som::Neuron& neuron = som_->neuron(i,j);

			// Get sampling quota for cluster based on size, S(i, j)
			unsigned n = (unsigned)(sizes[i][j] * dataset_->length() + 0.5);
            
            // Get training and testing sample quotas
            unsigned p = (unsigned)((double)n * trainingFraction_ + 0.5);
            unsigned q = (unsigned)((double)n * testingFraction_ + 0.5);
            	
            // Force selection of at least 1 train and test pair from a 
            // cluster
            if(p == 0) p = 1;
            if(q == 0) q = 1;
            	
            KEY::const_iterator key = neuron.indices().begin();
            for(unsigned i = 0; i < p && key != neuron.indices().end(); ++key, ++i)
               	trainingKey_.push_back(*key);
            
	        for(unsigned i = 0; i < q && key != neuron.indices().end(); ++key, ++i)
    	        testingKey_.push_back(*key);
            
        	// Add remaining points in cluster to validating set
            while(key != neuron.indices().end())
            {
               	validatingKey_.push_back(*key);
               	++key;
            }
        }

    }

    allocate(trainingKey_, training_);
    allocate(testingKey_, testing_);
    allocate(validatingKey_, validating_);
}
