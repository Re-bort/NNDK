#ifndef data_SomSampler_h
#define data_SomSampler_h

#include "Sampler.h"
#include "../ann/Som.h"
#include "../math/matops.h"

class SomSampler
    : public Sampler
{
	public:
								SomSampler();
								SomSampler(const Dataset& dataset,
										   const Som& som, 
										   const double trainingFraction = 1.0,
										   const double testingFraction = 0.8);
								SomSampler(const SomSampler& sampler);
								~SomSampler();

		const Som&				som(void) const;
		void 					setSom(Som& som);
		void					setOutlierThreshold(const unsigned& a);
        virtual void            resample(void);
        void                    resample(std::matrix<double> sizes);
		
	private:
		const Som *			    som_;
		unsigned				outlierThreshold_;
};

#endif // data_SomSampler_h
