#ifndef data_IvsFilter_h
#define data_IvsFilter_h

#include "../table"
#include "Encoder.h"
#include "../math/statistics.h"
#include "../math/vecops.h"
#include "Standardiser.h"

#define SERIES std::series<double>
#define TABLE std::table<double>
#define VECTOR std::vector<double>
#define MATRIX std::matrix<double>

class IvsFilter
{
	public:
		virtual                 ~IvsFilter();
	
        inline SERIES&          response();
        inline TABLE&           candidates(); 
		inline TABLE& 	        inputs(); 	 
		inline MATRIX& 	        candidateResiduals();	 
        inline VECTOR&          responseResidual();
		inline VECTOR&	        scores();	 
        inline VECTOR&          candidateScores(void);
        inline const double&	candidateScores(const unsigned i) const;
		inline VECTOR&	        inputScores(void);	 
		inline const double&	inputScores(const unsigned i) const;
		const unsigned 			s() const;
		void 					select(const unsigned j);
		virtual void 			calculateResiduals() = 0;
		virtual void 			calculateScores() = 0;

    protected:
        TABLE                   C_;
        SERIES                  y_;
        TABLE 			        S_;
		MATRIX                  V_;
        VECTOR                  u_;
		VECTOR 		            candidateScores_;
		VECTOR			        inputScores_;
        
                                IvsFilter(const TABLE& candidates, 
                                          const SERIES& response);
};

inline SERIES& IvsFilter::response()
{
    return y_;   
}

inline TABLE& IvsFilter::candidates() 
{ 
    return C_; 
}

inline TABLE& IvsFilter::inputs()     
{ 
    return S_;
}

inline MATRIX& IvsFilter::candidateResiduals()  
{ 
    return V_;
}

inline VECTOR& IvsFilter::candidateScores(void)
{
    return candidateScores_;
}

inline const double& IvsFilter::candidateScores(const unsigned i) const
{
	return candidateScores_[i];
}

inline VECTOR& IvsFilter::inputScores(void)    
{ 
    return inputScores_;
}

inline const double& IvsFilter::inputScores(const unsigned i) const
{
	return inputScores_[i];
}

#undef SERIES
#undef TABLE
#undef VECTOR
#undef MATRIX

#endif // IvsFilter_h

