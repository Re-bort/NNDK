#ifndef Grnn_h
#define Grnn_h

#include "../kde/Regression.h"
#include "../kde/Variance.h"

#define VECTOR std::vector<double>
#define MATRIX std::matrix<double>

class Grnn
    : public kde::Regression<Gaussian>
{
    public:
        class Trainer;
    
                                Grnn(const MATRIX& X, const VECTOR& y);
};

class VarGrnn
	: public kde::Variance<Gaussian>
	
{
	public:
								VarGrnn(const Grnn& grnn);
	
	private:
								VarGrnn();
};

class Grnn::Trainer
{
    public:
        typedef enum { brent, climb, eval } Algorithm;
    
    private:
        typedef double (*Error)(const VECTOR&, const VECTOR&);
    
    public:
                                Trainer(const MATRIX& X, const VECTOR& y);
        
        const MATRIX&           X(void) const;
        const VECTOR&           y(void) const;
        const double            error(Grnn& grnn);
        const double            error(Grnn& grnn, const double bandwidth);
        const double            tolerance(void) const;
        const double            minBandwidth(void) const;
        const double            maxBandwidth(void) const;
        const unsigned          maxIter(void) const;
        void                    setError(Error error);
        void                    setTolerance(const double tolerance);
        void                    setMaxBandwidth(const double& bandwidth);
        void                    setMinBandwidth(const double& bandwidth);
        void                    setMaxIter(const unsigned& maxIter);
        void                    setDefaultParameters(void);
        const double            train(Grnn& grnn, Algorithm algorithm = brent);
        
                        
    private:
        const MATRIX *          X_;
        const VECTOR *          y_; 
        VECTOR                  z_;
        Error                   error_;
        double                  tolerance_;
        double                  minBandwidth_;
        double                  maxBandwidth_;
        unsigned                maxIter_;
        
        void                    trainBrent(Grnn& grnn);
        void                    trainHillClimb(Grnn& grnn);
        void                    trainEnumerate(Grnn& grnn);
        void                    bracket(Grnn& grnn, 
                                  double& a, 
                                  double& b, 
                                  double& c);
};

#undef VECTOR
#undef MATRIX

#endif // ann_Grnn_h

