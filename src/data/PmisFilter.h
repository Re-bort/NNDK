#ifndef data_PmisFilter_h
#define data_PmisFilter_h

#include "IvsFilter.h"

#define SERIES std::series<double>
#define TABLE std::table<double>
#define VECTOR std::vector<double>
#define MATRIX std::matrix<double>

class PmisFilter 
    : public IvsFilter
{
	public:    
    							PmisFilter(const TABLE& candidates,
                                    const SERIES& response);
                                ~PmisFilter();
    
    	virtual void            calculateScores();
        virtual void 			calculateResiduals();
        
        const VECTOR            bootstrap(const unsigned B) const;
        const double            hampel(const unsigned j) const;
        const double            aic() const;
        const double            aicK() const;
        const double			trace() const;
        const double            rss() const;
};

#undef SERIES
#undef TABLE
#undef VECTOR
#undef MATRIX

#endif //PmisFilter_h
