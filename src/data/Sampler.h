#ifndef Sampler_h
#define Sampler_h

#include "Dataset.h"
#include "../math/sequence.h"
#include "../algo/sample.h"
#include "../rng/Random.h"

#define TABLE std::table<double> 
#define SERIES std::series<double>
#define KEY std::vector<unsigned>

class Sampler
{
    public:
        						Sampler();
								Sampler(const Dataset& dataset,
								  const double trainingFraction = 0.5,
								  const double testingFraction = 0.25);
								Sampler(const Sampler& sampler);
		virtual					~Sampler();
		
        const Dataset&          dataset() const;
        const Dataset&          training() const;
        const Dataset&          testing() const;
        const Dataset&          validating() const;
        const double            validatingFraction(void) const;
        const double            testingFraction(void) const;            
        const double            trainingFraction(void) const;
        const KEY&				trainingIndex() const;
        const KEY&				testIndex() const;
        const KEY&				validatingIndex() const;
        void                    setDataset(const Dataset& dataset);
        void                    setTrainingFraction(
                                  const double trainingFraction);
        void                    setTestingFraction(
                                  const double testingFraction);
        
        const unsigned          trainingSampleSize() const;
        const unsigned          testingSampleSize() const;
        const unsigned          validatingSampleSize() const;
        const unsigned          sampleSize() const;
        
        virtual void            resample(void);
        void                    allocate(KEY key, Dataset& sample);
        void 					allocate(Dataset& sample);
        
	protected:
        const Dataset *         dataset_;
        Dataset                 training_;
        Dataset                 testing_;
        Dataset                 validating_;
        double                  trainingFraction_;
        double 					testingFraction_;
        double					validatingFraction_;
        KEY                     trainingKey_;
        KEY                     testingKey_;
        KEY                     validatingKey_;
        
        
};

#undef TABLE
#undef SERIES
#undef KEY
#endif // Sampler_h
