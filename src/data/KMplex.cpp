#include "KMplex.h"

#define MATRIX std::matrix<double>
#define VECTOR std::vector<double>
#define KEY std::vector<unsigned>

KMplex::KMplex()
{
	// Empty constructor
}

KMplex::KMplex(const Dataset& dataset,
					const KMeans& kmeans, 
					const double sampleFraction,
					const double trainingFraction)
					: Sampler(dataset, sampleFraction, trainingFraction), 
					kmeans_(&kmeans), D_(dataset.length(), dataset.length(), 0.0),
					alloc_(Proportional)
{}

KMplex::~KMplex()
{
	// Empty destructor
}

void KMplex::setAllocation(AllocationType allocationType)
{
	alloc_ = allocationType;
}

void KMplex::resample()
{
	std::cout << "KMplex::resample" << std::endl;
	
	// Build dissimilarity matrix for clustered data X
	const MATRIX& X(*(kmeans_->X_));
	D_.resize(X.rows(), X.rows());

	std::cout << "\nKMplex: (Allocation rule: " 
		<< (unsigned)alloc_ << ")" << std::endl;
	for(unsigned i = 0; i < kmeans_->K(); ++i)
	{
		const KMeans::Cluster& cluster(kmeans_->cluster(i));

			if(cluster.size() > 0)
			{
				unsigned NTr = trainingQuota(cluster);
				unsigned NTe = testingQuota(cluster);
				unsigned NVa = validatingQuota(cluster);
			
				std::cout << "Cluster (" << i << ") N: " << cluster.size() <<
					" NTr: " << NTr << 
					" NTe: " << NTe << 
					" NVa: " << NVa << std::endl;
			
				// Calculate intra-cluster distances
				KEY indices = kmeans_->cluster(i).indices();
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
	
	allocate(trainingKey_, training_);
	allocate(testingKey_, testing_);
	allocate(validatingKey_, validating_);	
}


void KMplex::merge(KEY& S, KEY& T)
{
	for(KEY::iterator s = S.begin(); s != S.end(); ++s)
		T.push_back(*s);
}

void KMplex::initialise(KEY& T, KEY& S)
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
	T.erase(b);
	T.erase(a);
}

void KMplex::greedy(KEY& T, KEY& S)
{
	while(T.size() > 0)
	{ 
		S.push_back(T.back());
		T.pop_back();
	}
	return;
}

void KMplex::sample(KEY& T, KEY& S)
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

const unsigned KMplex::trainingQuota(const KMeans::Cluster& cluster) const
{
	unsigned quota;
	double den = 0.0;
	double num = cluster.size() * stdev(cluster.distances());
	switch(alloc_)
	{
		case Neyman:
			for(unsigned i = 0; i < kmeans_->K(); ++i)
			   		if(!kmeans_->cluster(i).empty())
            		{
                		den += (double)kmeans_->cluster(i).size() * 
             				stdev(kmeans_->cluster(i).distances());
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
			quota =  (unsigned)((double)cluster.size() 
		    	* trainingFraction_  + 0.5);
		    break;
		
	}

	// Training quota cannot be zero	
	if(trainingFraction_ > 0 && quota == 0)
		quota = 1;

	return quota;
}

const unsigned KMplex::testingQuota(const KMeans::Cluster& cluster) const
{
	unsigned quota;
	double den = 0.0;
	double num = cluster.size() * stdev(cluster.distances());
    switch(alloc_)
	{
		case Neyman:
			for(unsigned i = 0; i < kmeans_->K(); ++i)
            		if(!kmeans_->cluster(i).empty())
            		{
                		den += (double)kmeans_->cluster(i).size() * 
             				stdev(kmeans_->cluster(i).distances());
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
			quota =  (unsigned)(cluster.size() 
		    	* testingFraction_ + 0.5);
		    break;
	}
	
	return quota;
}

const unsigned KMplex::validatingQuota(const KMeans::Cluster& cluster) const
{
    int quota = cluster.size()
    	- trainingQuota(cluster) - testingQuota(cluster); 
    if(quota < 0)
    	quota = 0;

    return (unsigned)quota;
}

#undef MATRIX
#undef VECTOR
#undef KEY

