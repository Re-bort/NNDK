#ifndef data_SdhSampler_h
#define data_SdhSampler_h

#include "Sampler.h"
#include "../ann/Som.h"
#include "../ann/Sdh.h"

class SdhSampler
    : public Sampler
{
    public:
                                SdhSampler();
                                SdhSampler(const std::matrix<double>& database,
                                           const Som& som,
                                           const Sdh& sdh);
                                SdhSampler(const SdhSampler& sampler);
                                ~SdhSampler();

        // Overloaded sampling method
        using Sampler::draw;  // Unhides base functions of same name
        void                    draw(void);

    private:
        const Som *            som_;
        const Sdh *            sdh_;
};

#endif // data_SdhSampler_h



