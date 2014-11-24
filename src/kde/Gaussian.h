#ifndef kde_Gaussian_h
#define kde_Gaussian_h

#include "Kernel.h"
#include "../math/vecops.h"
#include "../math/constants.h"

#define VECTOR std::vector<double> 

class Gaussian1D
    : public Kernel1D
{
    public:
                                Gaussian1D();
                                Gaussian1D(const double& bandwidth);
                                ~Gaussian1D();
                                
        virtual void            setBandwidth(const double& bandwidth);
        virtual const double    width(void) const;
        virtual const double    operator () (const double& x) const;
};

class Gaussian2D
	: public Kernel2D
{
	public:
								Gaussian2D();
                                Gaussian2D(const double& bandwidth);
								~Gaussian2D();
								
		void					setBandwidth(const double& bandwidth);
		const double			volume(void) const;
		const double			operator () (const double& x, 
                                             const double& y) const;
};

class Gaussian
    : public Kernel
{
    public:
                                Gaussian();
                                Gaussian(const double& bandwidth);
                                ~Gaussian();
                                
        void                    setBandwidth(const double& bandwidth);
        const double            volume(const unsigned& d) const;
        const double            operator () (const VECTOR& x) const;
};

#undef VECTOR

#endif // kde_Gaussian_h
