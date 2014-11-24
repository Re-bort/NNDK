#include "DuplexSampler.h"
#include "../matrix"
#include "../math/matops.h"
#include "../math/vecops.h"
#include "Standardiser.h"

#define KEY std::vector<unsigned>

DuplexSampler::DuplexSampler()
{}

DuplexSampler::DuplexSampler(const Dataset& dataset,
							 const double trainingFraction,
							 const double testingFraction)
	: Sampler(dataset, trainingFraction, testingFraction), 
		D_(dataset.length(), dataset.length())
{
	// Calculate standardised distances
	std::matrix<double> X(dataset.X);
	// Include output variable in distance calculations
	X.push_backc(dataset.y);
	Standardiser encoder(X);
	encoder.encode(X);
	for(unsigned i = 0;	i < X.rows(); ++i)
		for(unsigned j = i + 1; j < X.rows(); ++j)
		{
			D_[i][j] = euclidean(X[i], X[j]);
			D_[j][i] = euclidean(X[i], X[j]);
		}
}

DuplexSampler::~DuplexSampler()
{}

void DuplexSampler::resample()
{
	trainingKey_.clear();
	testingKey_.clear();
	validatingKey_.clear();

	// Create list of items
	std::sequence<unsigned> key(0, dataset_->length() - 1);
	
	if(trainingSampleSize() > 0)
		initialiseSample(key, trainingKey_);

	if(testingSampleSize() > 0)	
		initialiseSample(key, testingKey_);

	if(validatingSampleSize() > 0)
		initialiseSample(key, validatingKey_);
	
	while(key.size() > 0)
	{
		if(trainingKey_.size() < trainingSampleSize())
		{
			kennardStoneSample(key, trainingKey_);
		}
	
		if(testingKey_.size() < testingSampleSize())
		{
			kennardStoneSample(key, testingKey_);
		}
		
		if(validatingKey_.size() < validatingSampleSize())
		{
			kennardStoneSample(key, validatingKey_);
		}
	}
	
	//T.decode(dataset_->X);
	allocate(trainingKey_, training_);
	allocate(testingKey_, testing_);
	allocate(validatingKey_, validating_);
}

void DuplexSampler::initialiseSample(KEY& T, KEY& S)
{
	if(T.size() < 2)
	{
		return;
	}
	
	KEY::iterator a = T.begin();
	KEY::iterator b = T.begin() + 1;
	double maxDist = 0.0;
	
	for(KEY::iterator t = T.begin(); t != T.end(); ++t)
	{
		for(KEY::iterator tt = t; tt != T.end(); ++tt)
		{
			double dist = D_[*t][*tt];
			if(dist > maxDist)	
			{
				maxDist = dist;
				a = t;
				b = tt;
			}
		}
	}
		
	// Copy object indices a and b into sample key S
	S.push_back(*a);
	S.push_back(*b);
	
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



void DuplexSampler::kennardStoneSample(KEY& T, KEY& S)
{
	// KLUDGE: Cannot K--S sample for two or less data, copy any data to S
	if(T.size() <= 2)
	{
		while(T.size() > 0)
		{ 
			S.push_back(T.back());
			T.pop_back();
		}
		return;
	}

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


#undef KEY


