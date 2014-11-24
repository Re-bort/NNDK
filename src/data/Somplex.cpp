#include "Somplex.h"

#define MATRIX std::matrix<double>
#define VECTOR std::vector<double>
#define KEY std::vector<unsigned>

Somplex::Somplex()
{
	// Empty constructor
}

Somplex::Somplex(const Dataset& dataset,
					const Som& som, 
					const double trainingFraction,
					const double testingFraction)
					: Sampler(dataset, trainingFraction, testingFraction), 
					som_(&som), D_(dataset.length(), dataset.length(), 0.0),
					alloc_(Proportional)
{}

Somplex::~Somplex()
{
	// Empty destructor
}

void Somplex::setAllocation(AllocationType allocationType)
{
	alloc_ = allocationType;
}

void Somplex::resample()
{
	// Build dissimilarity matrix for clustered data X
	const MATRIX& X(*(som_->X_));
	D_.resize(X.rows(), X.rows());

	for(unsigned i = 0; i < som_->rows(); ++i)
	{
		for(unsigned j = 0; j < som_->columns(); ++j)
		{
			const Som::Neuron& neuron(som_->neuron(i, j));

			if(neuron.size() > 0)
			{
				unsigned NTr = trainingQuota(neuron);
				unsigned NTe = testingQuota(neuron);
				unsigned NVa = validatingQuota(neuron);
			
				// Calculate intra-cluster distances
				KEY indices = som_->neuron(i, j).indices();
				for(KEY::iterator a = indices.begin(); a != indices.end(); ++a)
					for(KEY::iterator b = a + 1; b != indices.end(); ++b)
					{
						D_[*a][*b] = euclidean(X[*a], X[*b]);
						D_[*b][*a] = D_[*a][*b];
					}
		
				// Initialise samples
				KEY training, testing, validating;
				if(NTr > 0)
					initialise(indices, training);

				if(NTe > 0)
					initialise(indices, testing);
			
				if(NVa > 0)
					initialise(indices, validating);
			
				// Sample remaining points using Duplex sampling
				while(indices.size() > 0)
				{
					if(training.size() < NTr)
						sample(indices, training);
							
					if(testing.size() < NTe)
						sample(indices, testing);
			
					if(validating.size() < NVa)
						sample(indices, validating);
				}
	
				// Add intra-cluster samples to respective global samples	
				if(training.size() > 0)
					merge(training, trainingKey_);
			
				if(testing.size() > 0)
					merge(testing, testingKey_);
			
				if(validating.size() > 0)
					merge(validating, validatingKey_);
			}
		}
	}
	
	allocate(trainingKey_, training_);
	allocate(testingKey_, testing_);
	allocate(validatingKey_, validating_);	
}

void Somplex::merge(KEY& S, KEY& T)
{
	for(KEY::iterator s = S.begin(); s != S.end(); ++s)
		T.push_back(*s);
}

void Somplex::initialise(KEY& T, KEY& S)
{
	if(T.size() <= 2)
	{
		greedy(T, S);
		return;	
	}
	
	KEY::iterator a;
	KEY::iterator b;
	double dab = 0.0;

	for(KEY::iterator t = T.begin(); t != T.end(); ++t)
		for(KEY::iterator tt = t + 1; tt != T.end(); ++tt)
		{
			if(D_[*t][*tt] > dab)	
			{
				dab = D_[*t][*tt];
				a = t;
				b = tt;
			}
		}

	// Copy object indices a and b into sample key S
	S.push_back(*a);
	S.push_back(*b);
	
	// Remove object indices from key T
	//T.erase(b);
	//T.erase(a);
	// Remove index values from T, starting with largest first
	// NOTE: Erasing an element reduces the size of T by 1. 
	// If the second value erased initially referred to the last
	// element of T, then it points to element T in a [T-1] array 
	// causing an array bounds error.
	if(b > a)
	{
		T.erase(b);
		T.erase(a);
	}
	else
	{
		T.erase(a);
		T.erase(b);
	}
}

void Somplex::greedy(KEY& T, KEY& S)
{
	while(T.size() > 0)
	{ 
		S.push_back(T.back());
		T.pop_back();
	}
	return;
}

void Somplex::sample(KEY& T, KEY& S)
{
	
	if(T.size() <= 2)
	{
		greedy(T, S);
		return;
	}
	// NOTE: Cannot sample for two or less data, so copy any data to S
	
	// Indices of points a and b
	KEY::iterator a;
	KEY::iterator b;
	
	// Corresponding distances to points a and b, from c(S)
	double da = 0.0;
	double db = 0.0;
	for(KEY::iterator t = T.begin(); t != T.end(); ++t)
	{
		// Find shortest distance d(t,s)
		double d = large();
		for(KEY::iterator s = S.begin(); s != S.end(); ++s)
		{
			if(D_[*t][*s] < d)
				d = D_[*t][*s];
		}
		
		// Retain t if corresponds to first or second farthest from S
		if(d >= da)
		{
			b = a;		// Farthest object moves to second farthest
			db = da;
			a = t;		// New object t is farthest from s in S
			da = d;
		}
		else if(d >= db)	// New object t is second farthest from s in S
		{ 
			b = t;
			db = d;
		}
		else
		{
			; 			// Do nothing
		}
	}
		
	// Indices a and b are moved to sample, S
	S.push_back(*a);
	S.push_back(*b);

	// Erase items a and b, starting with last in sequence
	if(a > b)
	{
		T.erase(a);
		T.erase(b);
	}
	else
	{
		T.erase(b);
		T.erase(a);
	}
}

const unsigned Somplex::trainingQuota(const Som::Neuron& neuron) const
{
	unsigned quota;
	double den = 0.0;
	double num = neuron.size() * stdev(neuron.distances());
	switch(alloc_)
	{
		case Neyman:
			for(unsigned i = 0; i < som_->rows(); ++i)
        		for(unsigned j = 0; j < som_->columns(); ++j)
			   		if(!som_->neuron(i, j).empty())
            		{
                		den += (double)som_->neuron(i, j).size() * 
             				stdev(som_->neuron(i, j).distances());
            		}

			quota =  (unsigned)((num / den) * dataset_->length() *
				trainingFraction_) ;
			break;
		
		case Single:
			quota = 1;
			break;
		
		default:
			// Fall through

		case Proportional:
			quota =  (unsigned)((double)neuron.size() 
		    	* trainingFraction_ + 0.5);
		    break;
		
	}

	// Training quota cannot be zero	
	if(trainingFraction_ > 0 && quota == 0)
		quota = 1;

	return quota;
}

const unsigned Somplex::testingQuota(const Som::Neuron& neuron) const
{
	unsigned quota;
	double den = 0.0;
	double num = neuron.size() * stdev(neuron.distances());
    switch(alloc_)
	{
		case Neyman:
			for(unsigned i = 0; i < som_->rows(); ++i)
        		for(unsigned j = 0; j < som_->columns(); ++j)
            		if(!som_->neuron(i, j).empty())
            		{
                		den += (double)som_->neuron(i, j).size() * 
             				stdev(som_->neuron(i, j).distances());
            		}
		    
			quota =  (unsigned)((num / den) * dataset_->length() * 
				testingFraction_) ;
			break;
		
		case Single:
			quota = 1;
			break;
			
		default:
			// Fall through
		
		case Proportional:
			quota =  (unsigned)(neuron.size() 
		    	* testingFraction_ + 0.5);
		    break;
	}
	
	// Testing quota cannot be zero	
	if(testingFraction_ > 0 && quota == 0)
		quota = 1;

	return quota;
}

const unsigned Somplex::validatingQuota(const Som::Neuron& neuron) const
{
    int quota = neuron.size()
    	- trainingQuota(neuron) - testingQuota(neuron); 
    if(quota < 0)
    	quota = 0;

    return (unsigned)quota;
}

#undef MATRIX
#undef VECTOR
#undef KEY

