#ifndef kde_CityBlock_h
#define kde_CityBlock_h

#include "Kernel.h"

#define VECTOR std::vector<double>

class CityBlock1D
    : public Kernel1D
{
    public:
                                CityBlock1D();
                                CityBlock1D(const double& bandwidth);                   
                                ~CityBlock1D();
        
        void                    setBandwidth(const double& bandwidth);
        const double            width() const;
        const double            operator () (const double& x) const;
};

class CityBlock2D
    : public Kernel2D
{
    public:
                                CityBlock2D();
                                CityBlock2D(const double& bandwidth);                   
                                ~CityBlock2D();
        
        void                    setBandwidth(const double& bandwidth);
        const double            volume(void) const;
        const double            operator () (const double& x, 
                                             const double& y) const;
};

class CityBlock
    : public Kernel
{
    public:
                                CityBlock();
                                CityBlock(const double& bandwidth);                   
                                ~CityBlock();
        
        void                    setBandwidth(const double& bandwidth);
        const double            volume(const unsigned& d) const;
        const double            operator () (const VECTOR& x) const;
};

#undef VECTOR

#endif //kde_CityBlock_h
