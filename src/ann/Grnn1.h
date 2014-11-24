#ifndef Grnn1_h
#define Grnn1_h

public:
        typedef double (*ErrorFunction)(const std::vector<double>&, 
                                        const std::vector<double>&);
    
    public:
        class Optimisation
        {   
            public:
                Optimisation(Regression& regression, 
                             const std::vector<double>& x,
                             const std::vector<double>& y, 
                             ErrorFunction error = mse,
                             const double tolerance = 0.001)
                    : regression_(&regression), x_(&x), y_(&y), m_(y.size()), error_(error), 
                      tolerance_(tolerance)
                {}
        
                inline double tolerance(void) const 
                {
                    return tolerance_;  
                }
            
                void setTolerance(const double& tolerance)
                {
                    tolerance_ = tolerance;
                }
            
                const double operator () (const double& bandwidth)
                {   
                    regression_->K().setBandwidth(bandwidth);
                    m_ = (*regression_)(*x_);
                    return (*error_)(*y_, m_);
                }       
            
            private:
                Regression *                regression_;
                const std::vector<double> * x_;
                const std::vector<double> * y_;
                std::vector<double>         m_;
                ErrorFunction               error_;
                double                      tolerance_;

                Optimisation() // Hidden                
                {} 
            };

#endif // ann_Grnn1_h
