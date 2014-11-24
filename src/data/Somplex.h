#ifndef somplex_h
#define somplex_h

#include "Sampler.h"
#include "../ann/Som.h"

#define MATRIX std::matrix<double>
#define VECTOR std::vector<double>
#define KEY std::vector<unsigned>

class Somplex
	: public Sampler
{
	public:
		enum AllocationType {
				Proportional,
				Single,
				Neyman		
		};
	
	public:
							Somplex();
							Somplex(const Dataset& dataset,
									const Som& som, 
									const double trainingFraction = 0.50,
									const double testingFraction = 0.25);
							Somplex(const Somplex& sampler);
							~Somplex();
						
		void				resample();
		void				setAllocation(AllocationType allocationType);
						
	private:
		void				initialise(KEY& T, KEY& S);
		void 				sample(KEY& T, KEY& S);
		void				greedy(KEY& T, KEY& S);
		const unsigned 		trainingQuota(const Som::Neuron& neuron) const;
		const unsigned 		testingQuota(const Som::Neuron& neuron) const;
		const unsigned 		validatingQuota(const Som::Neuron& neuron) const;
		void 				merge(KEY& S, KEY& T);
		
			
	private:
		const Som *			som_;
		MATRIX				D_;
		AllocationType		alloc_;
};

#undef MATRIX
#undef VECTOR
#undef KEY
#endif


