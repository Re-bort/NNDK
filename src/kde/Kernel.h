#ifndef kde_Kernel_h
#define kde_Kernel_h

#include <vector>

#define VECTOR std::vector<double>

// Univariate (one-dimensional) kernel function class
class Kernel1D
{
    public:
        
        virtual                 ~Kernel1D();
        // Access methods
        const double&           bandwidth(void) const;
        virtual void            setBandwidth(const double& bandwidth) = 0;
        virtual const double    width() const = 0;
        virtual const double    operator () (const double& x) const = 0;
        
    protected:
        double                  bandwidth_;
        double                  scaledBandwidth_;
                                
                                Kernel1D();
                                Kernel1D(const Kernel1D& kernel);
};

class Kernel2D
{
    public:
        
        virtual                 ~Kernel2D();
        // Access methods
        const double&           bandwidth(void) const;
        virtual void            setBandwidth(const double& bandwidth) = 0;
        virtual const double    volume(void) const = 0;
        virtual const double    operator () (const double& x,
                                  const double& y) const = 0;
        
    protected:
        double                  bandwidth_;
        double                  scaledBandwidth_;
                                
                                Kernel2D();
                                Kernel2D(const Kernel2D& kernel);
};

// Multivariate (d-dimensional) kernel function class 
class Kernel
{
    public:
        
        virtual                 ~Kernel();
        // Access methods
        const double&           bandwidth(void) const;
        virtual void            setBandwidth(const double& bandwidth) = 0;
        virtual const double    volume(const unsigned& d) const = 0;
        virtual const double    operator () (const VECTOR& x) const = 0;
        
    protected:
        double                  bandwidth_;
        double                  scaledBandwidth_;
                                
                                Kernel();
                                Kernel(const Kernel& kernel);
};

#undef VECTOR

#endif // kde_Kernel_h
