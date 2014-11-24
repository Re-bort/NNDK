#ifndef KMplex_h
#define KMplex_h

#include "Sampler.h"
#include "../ann/KMeans.h"

#define MATRIX std::matrix<double>
#define VECTOR std::vector<double>
#define KEY std::vector<unsigned>

class KMplex
	: public Sampler
{
	public:
		enum AllocationType {
				Proportional,
				Single,
				Neyman		
		};
	
	public:
							KMplex();
							KMplex(const Dataset& dataset,
									const KMeans& kmeans, 
									const double sampleFraction = 1.0,
									const double trainingFraction = 0.8);
							KMplex(const KMplex& sampler);
							~KMplex();
						
		void				resample();
		void				setAllocation(AllocationType allocationType);
						
	private:
		void				initialise(KEY& T, KEY& S);
		void 				sample(KEY& T, KEY& S);
		void				greedy(KEY& T, KEY& S);
		const unsigned 		trainingQuota(const KMeans::Cluster& cluster) const;
		const unsigned 		testingQuota(const KMeans::Cluster& cluster) const;
		const unsigned 		validatingQuota(const KMeans::Cluster& cluster) const;
		void 				merge(KEY& S, KEY& T);
		
			
	private:
		const KMeans *		kmeans_;
		MATRIX				D_;
		AllocationType		alloc_;
		
};

#undef MATRIX
#undef VECTOR
#undef KEY

#endif // KMplex_h
