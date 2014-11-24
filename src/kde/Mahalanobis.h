#ifndef kde_Mahalanobis_h
#define kde_Mahalanobis_h

#include "Kernel.h"
#include "../math/matops.h"

#define VECTOR std::vector<double>
#define MATRIX std::matrix<double>

class Mahalanobis1D
    : public Kernel1D
{
    public:
                                Mahalanobis1D();
                                Mahalanobis1D(const double& bandwidth);
                                ~Mahalanobis1D();
        
        void                    setBandwidth(const double& bandwidth);
        const double            width(void) const;
        const double            operator () (const double& x) const;
};

class Mahalanobis2D
    : public Kernel2D
{
    public:
                                Mahalanobis2D();
                                Mahalanobis2D(const double& bandwidth,
                                  const double& varx = 1.0,
                                  const double& vary = 1.0,
                                  const double& varxy = 0.0);
                                ~Mahalanobis2D();
        
        void                    setBandwidth(const double& bandwidth);
        void                    setCovariance(const double& varx,
                                  const double& vary,
                                  const double& varxy);
        const double            volume(void) const;
        const double            operator () (const double& x,
                                  const double& y) const;
                                  
        const double            d(const double& x, const double& y) const;
    
    protected:
        MATRIX                  inverseCovariance_; 
        double                  determinant_;
        
        
};

class Mahalanobis
    : public Kernel
{
    public:
                                Mahalanobis();
                                Mahalanobis(const double& bandwidth,
                                            const MATRIX& covariance);
                                ~Mahalanobis();
        
        void                    setBandwidth(const double& bandwidth);
        void                    setCovariance(const MATRIX& covariance);
        const double            volume(const unsigned& d) const;
        const double            operator () (const VECTOR& x) const;
    
    private:
        MATRIX                  inverseCovariance_; 
        double                  determinant_;
};

#undef VECTOR
#undef MATRIX

#endif //kde_Mahalanobis_h
