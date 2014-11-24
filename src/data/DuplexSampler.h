#ifndef DuplexSampler_h
#define DuplexSampler_h

#include "Sampler.h"

#define KEY std::vector<unsigned>

class DuplexSampler
	: public Sampler
{
		public:
								DuplexSampler();
								DuplexSampler(const Dataset& dataset,
										   const double trainingFraction = 0.5,
										   const double testingFraction = 0.25);
								DuplexSampler(const DuplexSampler& sampler);
								~DuplexSampler();

		virtual void            resample(void);
		
	private:
		void					kennardStoneSample(KEY& T, KEY& S);
		void					initialiseSample(KEY& T, KEY& S);
									
		std::matrix<double>		D_;
};

#undef KEY

#endif //DuplexSampler_h
