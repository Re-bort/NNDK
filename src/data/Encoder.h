#ifndef Encoder_h
#define Encoder_h

#include "../matrix"
#include "Dataset.h"

class Encoder 
{
	public:
        class Codon
        {
            public:
                virtual            ~Codon();
                
                virtual void       encode(double& x) = 0;    
                virtual void       decode(double& x) = 0;    
        };
    
    public:	
		typedef std::vector<Codon *>::iterator iterator;
		
		virtual                    ~Encoder();
		
		Codon&					   codon(unsigned i);
		void			           encode(std::matrix<double>& X);
      	void 			           encode(std::vector<double>& x, 
                                          unsigned i = 0);
       	void 			           decode(std::matrix<double>& X);
       	virtual void 			   decode(std::vector<double> x, 
                                          unsigned i = 0);

    protected:
        std::vector<Codon *>       codons_;
};

#endif // Encoder_h

